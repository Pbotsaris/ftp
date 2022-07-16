#include "server.hpp"
#include "logger.hpp"
#include "reply.hpp"

/* TODO:
 *   - Convert Service to work with Server.
 *   - Service will take a reference to the control connection to respond.
 *   - Server will move the request object into Service constructor.
 *   - Remove control Loop and Auth from the Service class. Service obj only
 * executes and responds to a single request.
 *   - Create a Authentication class to handle user login and login states of
 * individual connections.
 *   - Clear Exceptions from connection. Error handling using return values.
 *
 *   - Create threadpool to execute services concurrently.
 *
 *   - Maybe the connection can have a `busy state` when being used by a thread.
 *
 *   -  Rename is a problem to be solved still.
 */

const std::size_t Server::SINGLE_CONNECTION = 1;

Server::Server(int t_port) : m_port(t_port), m_awaiting(t_port) {
  m_awaiting.setup_and_listen();
};

void Server::new_connection() {
  /* if an accept is successful setup another connection to await */
  if (m_awaiting.accept_connection(Connection::await::True)) {

    /* move connected connection to the m_connections vector */
    m_awaiting.handshake();
    m_connections.push_back(std::move(m_awaiting));

    /* create new connection and listen */
    m_awaiting = ClientConn(m_port);
    m_awaiting.setup_and_listen();
  }
}

void Server::main_loop() {

  while (true) {

    Request req;
    bool result = receive(req);

    if (result && load_req_with_conn_info(req)) {
      Service service(get_connection(req));
      service.work(req); 

    } else {
      LOG_ERROR("Could not start service.");

    }
  }
}

bool Server::receive(Request &t_req) {

  if (m_connections.size() == SINGLE_CONNECTION) {
    return single_connection(t_req);
  };

  return multiple_connections(t_req);
}

bool Server::respond(Request &t_req) {

  if (conn_exist(t_req)) {
     get_connection(t_req).respond(t_req);
    return true;
  } else {
    LOG_ERROR("Could not respond. Requested connection is out of bounds.");
    return false;
  }
}

/****** Helpers *****/

bool Server::single_connection(Request &t_req) {
  t_req.m_conn_index = 0;

  /* single connection will block */
  return get_connection_at(0).receive(t_req, Connection::await::True);
}

bool Server::multiple_connections(Request &t_req) {
  int index = 0;
  /* scan through connections to receive */
  for (auto &conn : m_connections) {
    if (conn.receive(t_req, Connection::await::True)) {
      t_req.m_conn_index = index;

      return true;
    }
    index++;
  }
  return false;
}

bool Server::load_req_with_conn_info(Request &t_req) const {

  if (conn_exist(t_req)) {
    t_req.m_disk = m_connections.at(t_req.m_conn_index).get_disk();
    t_req.m_logged_in = m_connections.at(t_req.m_conn_index).is_logged_in();
    t_req.m_current_user = m_connections.at(t_req.m_conn_index).get_user();

    controllers::DiskManager::init(t_req.m_disk);
    return true;
  } else {
    LOG_ERROR( "Could not load request info. Requested connection is out of bounds. %lu - %lu", t_req.m_conn_index, m_connections.size());
    return false;
  }
}

ClientConn &Server::get_connection(const Request &t_req) noexcept {
  return m_connections.at(t_req.m_conn_index);
}

ClientConn &Server::get_connection_at(const std::size_t t_index) noexcept {
  return m_connections.at(t_index);
}

bool Server::conn_exist(const Request &t_req) const noexcept {
  return t_req.m_conn_index < m_connections.size();
}

bool Server::conn_exist(const std::size_t t_index) const noexcept {
  return t_index < m_connections.size();
}


