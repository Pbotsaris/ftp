#include "connection.hpp"
#include "connection_poll.hpp"
#include "logger.hpp"

/* C headers */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace networking;

static const int QUEUE_SIZE = 5;

/********* Constructor **********/

Connection::Connection(int t_port, conn_mode t_mode)
    : m_port(t_port), m_mode(t_mode), m_connected_socket(-1){

  if(!create_socket())
      throw "Could not instanciate Connection.";

};

Connection::~Connection() {
  if(m_connected_socket > 0)
      close(m_connected_socket);

  if (m_mode == passive) // shuts down listening connection
    shutdown(m_local_socket, SHUT_RDWR);

}

/********* Conn config & state **********/

bool Connection::set_socket_options() {
  int opt = 1;

  int res_addr = setsockopt(m_local_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int res_port = setsockopt(m_local_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  if (res_addr < 0 || res_port < 0){
    LOG_ERROR("Invalid option");
    return false;
  }

  return true;
}

void Connection::config_addr() {
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY; /* localhost when no IP is provided */
  m_address.sin_port = htons(m_port);
}

void Connection::config_addr(const std::string &t_ip) {
  inet_pton(AF_INET, t_ip.c_str(), &m_address.sin_addr); /* converts IP string */

  m_address.sin_port = htons(m_port);
  m_address.sin_family = AF_INET;
}

bool Connection::bind_socket() {

  if (m_mode == active){
    LOG_ERROR("Cannot bind an active connection.");
    return false;
  }

  int res =
      bind(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
           sizeof(m_address));

  if (res < 0) {
    LOG_ERROR("Error Binding Socket.");
    return false;
  }

  return true;
}

bool Connection::connect_socket() {

  if (m_mode == passive){
    LOG_ERROR("Cannot call \'connect\' with passive connection");
    return false;
  }

  m_connected_socket = connect(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address), sizeof(m_address));

  if (m_connected_socket < 0) {
    LOG_ERROR("error could not connect to socket");
    return false;
  }

  LOG_INFO("Connected to client socket successfully.");
  return true;
}

bool Connection::socket_listen() {
  if (m_mode == active){
    LOG_ERROR("Cannot listen on an active connection");
    return false;
  }

  listen(m_local_socket, QUEUE_SIZE);

  LOG_INFO("Listening on port %d.", m_port);
  return true;
}


int Connection::accept_connection() {

  if (m_mode == active) {
    LOG_ERROR("Cannot \'accept\' in an active connection");
    return -1;
  }

  socklen_t client_addr_len = sizeof(m_address);

  m_connected_socket = accept(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address), &client_addr_len);

  if (m_connected_socket < 0) {
    LOG_ERROR("Failed to accept socket connection.");
    return -1;
  }

  LOG_INFO("Accepted connection at socket %d.",  m_connected_socket);
  return m_connected_socket;
}

bool Connection::reconnect() {
  if (m_mode == active) {
    shutdown(m_local_socket, SHUT_RDWR);
  } else {
    close(m_connected_socket);
  }
  /* creates a new socket after shutdown and clear IP address */
 return create_socket();
}

bool Connection::make_passive_and_listen(int port) {
  m_port = port;
  bool res = reconnect();
  m_mode = passive;
  m_connected_socket = 0;
  config_addr();

  return res && bind_socket() && socket_listen();
}

/********* getters and setters **********/

int Connection::get_local_socket() { return m_local_socket; }

int Connection::get_connected_socket() { return m_connected_socket; }

int Connection::get_port() { return m_port; }
void Connection::set_port(int t_port) { m_port = t_port; }


conn_mode Connection::get_mode() { return m_mode; }

void Connection::set_mode(conn_mode t_mode) { m_mode = t_mode; }

/* Private */

bool Connection::create_socket() {
  bzero(&m_address, sizeof(m_address));

  m_local_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_local_socket < 0) {
    LOG_ERROR("Unable to create socket");
    return false;
  }

  return true;
}
