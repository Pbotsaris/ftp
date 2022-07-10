#include "connection.hpp"
#include "connection_poll.hpp"
#include "logger.hpp"

/* C headers */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


using namespace networking;

/* Max read length in control */
static const int BUFFER_SIZE = 100;
static const int MAX_READ_SIZE = BUFFER_SIZE - 2;
/* Buffer size which data is read from socket */
static const int TRANSFER_READ_LENGTH = 1048; // 1MB
/* max request queuing in sockets */
static const int QUEUE_SIZE = 5;

/********* Constructor **********/

Connection::Connection(int t_port, conn_mode t_mode, conn_type t_type)

    : m_port(t_port), m_mode(t_mode), m_connected_socket(0), m_type(t_type) {
  /* init to 0 */

  create_socket();

  if (m_local_socket < 0)
    throw "invalid socket";
};

Connection::~Connection() {
  close(m_connected_socket);

  if (m_mode == passive) // shuts down listening connection
    shutdown(m_local_socket, SHUT_RDWR);
}

/********* Conn config & state **********/

void Connection::set_socket_options() {
  int opt = 1;

  int res_addr =
      setsockopt(m_local_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int res_port =
      setsockopt(m_local_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  if (res_addr < 0 || res_port < 0)
    throw "Invalid option\n";
}

void Connection::config_addr() {
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY; /* localhost when no IP is provided */
  m_address.sin_port = htons(m_port);
}

void Connection::config_addr(const std::string &t_ip) {
  inet_pton(AF_INET, t_ip.c_str(),
            &m_address.sin_addr); /* converts IP string */

  m_address.sin_family = AF_INET;
  m_address.sin_port = htons(m_port);
}

void Connection::bind_socket() {

  if (m_mode == active)
    throw "Cannot bind an active connection\n";

  int res =
      bind(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
           sizeof(m_address));

  if (res < 0)
    throw "Error Binding Socket.\n";
}

void Connection::connect_socket() {

  if (m_mode == passive)
    throw "Cannot call \'connect\' with passive connection\n";

  m_connected_socket =
      connect(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
              sizeof(m_address));

  if (m_connected_socket < 0)
    throw "error could not connect to socket\n";

  LOG_INFO("Connected to client socket successfully.");
}

void Connection::accept_connection() {

  if (m_mode == active)
    throw "Cannot \'accept\' in an active connection\n";

  socklen_t client_addr_len = sizeof(m_address);

  m_connected_socket =
      accept(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
             &client_addr_len);

  if (m_connected_socket < 0)
    throw "error accepting a connection\n";

  LOG_INFO("Accepted connection to socket.");
}

void Connection::socket_listen() {
  if (m_mode == active)
    throw "Cannot listen on an active connection\n";

  listen(m_local_socket, QUEUE_SIZE);

  LOG_INFO("Listening on port %d.", m_port);
}

void Connection::reconnect() {
  shutdown(m_local_socket, SHUT_RDWR);
  /* creates a new socket after shutdown and clear IP address */
  create_socket();
}

/********* getters and setters **********/

int Connection::get_port() { return m_port; }

void Connection::set_port(int t_port) { m_port = t_port; }

void Connection::set_mode(conn_mode t_mode) { m_mode = t_mode; }

void Connection::set_type(conn_type t_type) { m_type = t_type; }

Connection::conn_type Connection::get_type() { return m_type; }

/********* CONTROL **********/

void Connection::receive(Request &t_req) {

  char read_buffer[BUFFER_SIZE] = {0};
  int read_count = 0;

  while (1) {

    int read_size = recv(m_connected_socket, &read_buffer[read_count], 1, 0);

    if (read_count >= MAX_READ_SIZE) {
      t_req.m_raw.append(read_buffer);
      memset(read_buffer, 0, BUFFER_SIZE);
      read_count = 0;
    }

    if (read_size < 0)
      throw "error receiving data\n";

    if (read_buffer[read_count] == '\n')
      break;

    read_count++;
  }

  t_req.m_raw.append(read_buffer);

  LOG_DEBUG("Message Received: %s", t_req.m_raw.c_str());
}

void Connection::respond(Request &t_req) {

  std::string msg;

  if (t_req.m_reply_msg.empty())
    msg = reply::messages[t_req.m_reply];
  else
    msg = reply::Utils::append_message(t_req.m_reply, t_req.m_reply_msg);

  int res = send(m_connected_socket, msg.c_str(), msg.size(), 0);

  if (res < 0)
    throw "Could not respond to client\n";
}

/********* DATA **********/

void Connection::transfer_send(Request &t_req) {

  if (m_type == Connection::ascii)
    transfer_ascii(t_req);

  else
    transfer_image(t_req);

  LOG_INFO("Closing data connection.");

  reconnect();
}

DatafromClientTuple Connection::transfer_receive(Request &t_req) {

  std::uintmax_t total_length = 0;
  std::queue<TransferData> queue;
  ConnectionPoll conn_poll(m_local_socket);

  while (1) {

    if (conn_poll.has_timeout()) { /* data connection should never block */
      t_req.m_valid = false;
      break;
    }

    TransferData data;

    data.m_buffer = new char[TRANSFER_READ_LENGTH]();
    int read_length =
        recv(m_local_socket, data.m_buffer, TRANSFER_READ_LENGTH, 0);

    if (read_length == -1) {
      LOG_ERROR("There was a problem reading from client.");
      t_req.m_valid = false;
      delete[] data.m_buffer;
      break;
    }

    data.m_length = static_cast<std::uintmax_t>(read_length);

    /* recv returns 0 when client brakes connection */
    if (data.m_length == 0) {
      delete[] data.m_buffer;
      break;
    }

    total_length += data.m_length;
    queue.push(std::move(data));
  }

  LOG_DEBUG("Read %lu bytes from %s.", total_length, t_req.m_argument.c_str());

  ImageBuffer data = consolidate_data(queue, total_length);

  create_socket();

  return DatafromClientTuple(std::move(data), total_length);
}


void Connection::create_socket() {

  bzero(&m_address, sizeof(m_address));
  m_local_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_local_socket < 0)
    throw "invalid socket";
}

void Connection::transfer_ascii(Request &t_req) {

  int res = send(m_local_socket, t_req.m_data.m_ascii.c_str(),
                 t_req.m_data.m_ascii.size(), 0);

  if (res < 0) {
    LOG_ERROR("Could not transfer ASCII data to client.");
    t_req.m_valid = false;
  }
}

void Connection::transfer_image(Request &t_req) {

  int res = send(m_local_socket, t_req.m_data.m_image.get(),
                 t_req.m_data.m_image_size, 0);

  if (res < 0) {
    LOG_ERROR("Could not transfer Image/Binary data to client.");
    t_req.m_valid = false;
  }
}

ImageBuffer Connection::consolidate_data(std::queue<TransferData> t_data,
                                         std::uintmax_t t_total_length) {

  ImageBuffer consolidated_data(new char[t_total_length]);
  std::size_t cursor = 0;

  while (!t_data.empty()) {
    TransferData trans_data = t_data.front();
    t_data.pop();
    memmove(consolidated_data.get() + cursor, trans_data.m_buffer,
            trans_data.m_length);
    delete[] trans_data.m_buffer;
    cursor += trans_data.m_length;
  };

  return consolidated_data;
}

