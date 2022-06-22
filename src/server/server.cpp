#include "../../include/server.h"
#include <sys/socket.h>

Server::Server(int t_port, int t_queue_size)
    : m_port(t_port), m_queue_size(t_queue_size), m_connected_socket(0) {

  /* init to 0 */
  bzero(&m_server_addr, sizeof(m_server_addr));
  bzero(&m_client_addr, sizeof(m_client_addr));

  m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_socket < 0)
    throw "invalid socket";
};

void Server::set_socket_options() {
  int opt = 1;

  int res_addr = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int res_port = setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  if (res_addr < 0 || res_port < 0)
    throw "Invalid option\n";
}

void Server::config_addr() {
  m_server_addr.sin_family = AF_INET;
  m_server_addr.sin_addr.s_addr = INADDR_ANY; /* localhost when no IP is provided */
  m_server_addr.sin_port = htons(m_port);
}

void Server::config_addr(std::string &t_ip) {
  inet_pton(AF_INET, t_ip.c_str(), &m_server_addr.sin_addr); /* converts IP string */

  m_server_addr.sin_family = AF_INET;
  m_server_addr.sin_port = htons(m_port);
}

void Server::bind_socket() {

  int res = bind(m_socket, reinterpret_cast<struct sockaddr *>(&m_server_addr), sizeof(m_server_addr));

  if(res < 0)
    throw "Error Binding Socket.\n";
}

void Server::slisten() { listen(m_socket, m_queue_size); }

void Server::receive() {

  socklen_t client_addr_len = sizeof(m_client_addr);

  int m_connected_socket =
      accept(m_socket, reinterpret_cast<struct sockaddr *>(&m_client_addr),
             &client_addr_len);

  if (m_connected_socket < 0)
    std::cout << "error accepting"
              << "\n";

  char buffer[256] = {0};
  recv(m_connected_socket, buffer, 256, 0);

  std::cout << buffer << "\n";

  send(m_connected_socket, "Received!",9 , 0);

}
