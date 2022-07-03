#include "connection.hpp"

using namespace networking;

/*  This constants will impact how many times a c++ string gets appended  
 *  larger buffer will yield less appends.
 */

static const int BUFFER_SIZE = 100; 
static const int MAX_READ_SIZE = BUFFER_SIZE - 2;

/* This QUEUE_SIZE configures the number of queuing requests for each connection */
static const int QUEUE_SIZE = 5;

Connection::Connection(int t_port, conn_mode t_mode)
    : m_port(t_port), m_mode(t_mode), m_connected_socket(0) {

  /* init to 0 */
  bzero(&m_address, sizeof(m_address));
  m_local_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_local_socket < 0)
    throw "invalid socket";
};

Connection::~Connection() {
  close(m_connected_socket);

  if (m_mode == passive) // shuts down listening connection
    shutdown(m_local_socket, SHUT_RDWR);
}

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
  inet_pton(AF_INET, t_ip.c_str(), &m_address.sin_addr); /* converts IP string */

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

void Connection::socket_listen() {
  if (m_mode == active)
    throw "Cannot listen on an active connection\n";

  listen(m_local_socket, QUEUE_SIZE);
  
  LOG_INFO("Listening on port %d ...\n", m_port);
}

void Connection::connect_socket() {

  if (m_mode == passive)
    throw "Cannot call \'connect\' with passive connection\n";

  m_connected_socket =
      connect(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
              sizeof(m_address));

  if (m_connected_socket < 0)
    throw "error could not connect to socket\n";
}

void Connection::accept_connection() {

  if (m_mode == active)
    throw "Cannot \'accept\' in an active connection\n";

  socklen_t client_addr_len = sizeof(m_address);

  m_connected_socket =
      accept(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
             &client_addr_len);

  if (m_connected_socket < 0)
    throw "error accepting connecting\n";


  LOG_INFO("Accepted connection to socket.\n");

}

void Connection::receive(Request &t_req) {

  char read_buffer[BUFFER_SIZE] = {0};
  int read_count = 0;

 while(1){

   int read_size = recv(m_connected_socket, &read_buffer[read_count], 1, 0);


   if(read_count >= MAX_READ_SIZE) {
      t_req.m_raw.append(read_buffer);
      memset(read_buffer, 0, BUFFER_SIZE);
      read_count = 0;
   }

   if (read_size < 0)
     throw "error receiving data\n";

   if(read_buffer[read_count] == '\n')
       break;

   read_count++;
 }

  t_req.m_raw.append(read_buffer);

   LOG_DEBUG("Message Received: %s", t_req.m_raw.c_str());
}

void Connection::respond(Request &t_req) {

  std::string msg;

  if(t_req.m_reply_msg.empty())
     msg = reply::messages[t_req.m_reply];
  else
    msg = reply::Utils::append_message(t_req.m_reply, t_req.m_reply_msg);

  int res = send(m_connected_socket, msg.c_str(), msg.size(), 0);

  if(res < 0)
    throw "Could not respond\n";
}

int Connection::get_port(){
  return m_port;
}

void Connection::set_port(int t_port){
  m_port = t_port;
}

void Connection::set_mode(conn_mode t_mode){
  m_mode = t_mode;
}
