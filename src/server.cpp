#include "server.hpp"
#include "logger.hpp"
#include "reply.hpp"
#include <unistd.h>

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

Server::Server(int t_port)
    : m_conn(t_port), m_poll(m_conn.get_local_socket()){

  if (!m_conn.setup_and_listen()) {
    throw "Could not start server. Existing ...\n";
  }
}

bool Server::connect() {
  int connected_socket = m_conn.accept_connection();
  if (connected_socket > 0 && m_conn.handshake()) {

    /* creates a new service */
    m_services.insert(std::make_pair(connected_socket, Service(connected_socket)));

    /* adds socket descriptor to the poll system */
    m_poll.add_socket(connected_socket);
    return true;
  }
  return false;
}

void Server::main_loop() {

  while (true) {

    if(m_poll.accept_awaits()){
        LOG_DEBUG("called create service");
        create_service();
    }

    int pending_socket = scan_sockets_for_requests();

    if(pending_socket > 0){
      bool quit = m_services.at(pending_socket).work();

      if(quit){
        quit_service(pending_socket);
      }
    } 
      LOG_DEBUG("tick.");
  }

}

void Server::create_service(){
   if(!connect())
      LOG_ERROR("failed to create service.");
}

int Server::scan_sockets_for_requests(){
    return m_poll.select_socket_with_events();
}

void Server::quit_service(int t_socket){
    m_poll.remove_socket(t_socket);
    m_services.erase(t_socket);
}

