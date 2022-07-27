#include "server.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"
#include "reply.hpp"
#include <unistd.h>

/* TODO:
 *
 *   - check removing a service logic. hasn't been tested yet .
 *   - work on threads.
 *   -  Rename is a problem to be solved still.
 */

Server::Server(int t_port)
    : m_conn(t_port), m_poll(m_conn.get_local_socket()),

      /* max number of connection will instruct the max number of possible
       * Services that its turn will dictate the number of possible futures.
       * Note that the poll contains a single local_socket connection and only
       * connected_sockets spawn Services. therefore we need to subtract 1 to
       * take account the local socket from the connection_poll.
       *
       * */
      m_futures(m_poll.get_max_connections() - 1) {

  if (!m_conn.setup_and_listen()) {
    throw "Could not start server. Existing ...\n";
  }
}

bool Server::connect() {
  int connected_socket = m_conn.accept_connection();
  if (connected_socket > 0 && m_conn.handshake()) {

    /* creates a new service */
    m_services.insert(
        std::make_pair(connected_socket, Service(connected_socket)));

    /* adds socket descriptor to the poll system */
    m_poll.add_socket(connected_socket);
    return true;
  }
  return false;
}

void Server::main_loop() {

  while (true) {

    if (m_poll.accept_awaits()) {
      create_service();
    }

    int pending_socket = scan_sockets_for_requests();

    if (pending_socket > 0) {
      auto task = [=] () -> int { return m_services.at(pending_socket).work(); };

      m_futures.push_back(m_tpool.submit(std::move(task)));

      int quit = poll_futures();

      if (quit > 0) {
        quit_service(pending_socket);
      }
    }
  }
}

void Server::create_service() {
  bool res = connect();
  if (!res) {
    LOG_ERROR("failed to create service.");
  }
}

int Server::poll_futures() {
  for (auto &future : m_futures) {
    if (is_future_ready(future)) {

      LOG_INFO("future was ready!");

      return future.get();
    }
  }

  return -1;
}

int Server::scan_sockets_for_requests() {
  return m_poll.select_socket_with_events();
}

void Server::quit_service(int t_socket) {
  m_poll.remove_socket(t_socket);
  m_services.erase(t_socket);
}

bool Server::is_future_ready(std::future<int> const &t_future) {
  bool res = false;

  try {
  res = t_future.wait_for(std::chrono::seconds(1)) ==
         std::future_status::ready;
  } catch(...){
   
  }
  return res;
}
