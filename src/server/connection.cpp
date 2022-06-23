#include "../../include/connection.hpp"
#include <sys/socket.h>

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
}

void Connection::connect_socket() {

  if (m_mode == passive)
    throw "Cannot call connect with passive connection\n";

  m_connected_socket =
      connect(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
              sizeof(m_address));

  if (m_connected_socket < 0)
    throw "error could not connect to socket\n";
}

void Connection::accept_connection() {

  if (m_mode == active)
    throw "Cannot accept in an active connection\n";

  socklen_t client_addr_len = sizeof(m_address);

  std::cout << "Waiting to accept\n";

  m_connected_socket =
      accept(m_local_socket, reinterpret_cast<struct sockaddr *>(&m_address),
             &client_addr_len);

  std::cout << "accepted\n";

  if (m_connected_socket < 0)
    throw "error accepting connecting\n";
}

void Connection::receive() {
  char buffer[256] = {0};

  int res = recv(m_connected_socket, buffer, 256, 0);
  std::cout << buffer << "\n";

  if (res < 0)
    throw "error receiving data\n";
}

void Connection::respond(const std::string &msg) {
  send(m_connected_socket, msg.c_str(), msg.size(), 0);
}
