#include "server.hpp"
#include "thread_pool.hpp"
#include "logger.hpp"
#include "reply.hpp"
#include <unistd.h>
#include <chrono>

static int ITERATIONS_UNTIL_THROTTLE  = 20;

Server::Server(int t_port)
    : m_conn(t_port), m_poll(m_conn.get_local_socket()), m_no_activity(ITERATIONS_UNTIL_THROTTLE){
  if (!m_conn.setup_and_listen()) {
    throw "Could not start server. Existing ...\n";
  }
}

bool Server::connect() {

  int connected_socket = m_conn.accept_connection();
  if (connected_socket > 0 && m_conn.handshake()) {

    m_services.insert(
        /* creates a new service */
        std::make_pair(connected_socket, Service(connected_socket)));

    /* adds socket descriptor to the poll system */
    m_poll.add_socket(connected_socket);
    return true;
  }
  return false;
}

void Server::main_loop() {

  /* Any new connect request? Then create a service. */
  while (true) {

    if (m_poll.accept_awaits()) {
      create_service();
    }

    throttle(); /* no activity then throttle this loop */

    /* scans ConnectionPoll for sockets with awaiting messages and return respective socket fd. */
    int pending_socket = m_poll.select_socket_with_events();

    /* create a Service and run it on a an available ThreadPool thread. */
    if (pending_socket > 0) {

      /* stop throttle when has activity */
       m_no_activity = ITERATIONS_UNTIL_THROTTLE;

      auto task = [=] () -> int { return m_services.at(pending_socket).work(); };

     /* Store pending result in a future. We need to know if there was a quit command to destroy a Service. */
      m_futures.push_back(m_tpool.submit(std::move(task)));

    /* Check all futures in m_futures for any futures that might have resolved. result socket fd to quit or -1. */
      int quit = poll_futures();

      if (quit > 0) {
        /* Destroy a Service */
        quit_service(pending_socket);
      }
    } else {
         
       if(m_no_activity > 0){
        m_no_activity--;
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

void Server::quit_service(int t_socket) {
  m_poll.remove_socket(t_socket);
  m_services.erase(t_socket);
}

int Server::poll_futures() {

  int index = 0;

  for (auto &future : m_futures) {
    if (is_future_ready(future)) {

      int res = future.get();
      m_futures.erase(m_futures.begin() + index); 

      return res;
    }

    index++;
  }

  return -1;
}

bool Server::is_future_ready(std::future<int> const &t_future) {
  bool res = false;

  try {
  res = t_future.wait_for(std::chrono::seconds(1)) ==
         std::future_status::ready;
  } catch(...){
    LOG_ERROR("Error with future state");
  }
  return res;
}


void Server::throttle(){

  if(m_no_activity == 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
  }

  else if(m_no_activity > 0 && m_no_activity <= 10) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }

  else if(m_no_activity > 10 && m_no_activity <= 15) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

}
