#include "server.hpp"
#include "logger.hpp"
#include "reply.hpp"

/* TODO:
 *
 *  - test all commands for bugs
 *  - include connected socket to the polling system.
 *  - create logic to chose service to work depending on the connected socket.
 *  - maybe well be better off keeping the services in a map where the key is
 the connected socket descriptor.
 *  - need to create logic to close the connected socket when in the service
 destructor.
 *  - expand to multiple services

 *   -  Rename is a problem to be solved still.
 */

const std::size_t Server::SINGLE_CONNECTION = 1;

Server::Server(int t_port)
    : m_port(t_port), m_conn(t_port), m_poll(m_conn.get_local_socket()) {

  if (!m_conn.setup_and_listen()) {
    throw "Could not start server. Existing ...\n";
  }
}

bool Server::connect() {
  int connected_socket = m_conn.accept_connection();

  if (connected_socket > 0 && m_conn.handshake()) {
    m_services.push_back(Service(connected_socket));
    m_poll.add_socket(connected_socket);
    return true;
  }
  return false;
}

void Server::main_loop() {

  while (true) {
    m_services.at(0).work();
  }
}
