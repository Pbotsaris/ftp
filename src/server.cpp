#include "server.hpp"


/* TODO: 
 *   - Convert Service to work with Server. 
 *   - Service will take a reference to the control connection to respond.
 *   - Server will move the request object into Service constructor.
 *   - Remove control Loop and Auth from the Service class. Service obj only executes and responds to a single request.
 *   - Create a Authentication class to handle user login and login states of individual connections.
 *   - Clear Exceptions from connection. Error handling using return values.
 *
 *   - Create threadpool to execute services concurrently.
 *
 *   - Maybe the connection can have a `busy state` when being used by a thread.
 *
 *   -  Rename is a problem to be solved still.
 */

const int Server::SINGLE_CONNECTION = 1;

Server::Server(int t_port) : m_port(t_port), m_awaiting(t_port) {

  m_awaiting.setup_and_listen();
};

void Server::accept_connection() {

  /* if an accept is successfull setup another connection to await */
  if (m_awaiting.accept_connection(Connection::is_blocking::False)) {
    m_connections.push_back(std::move(m_awaiting));
    m_awaiting = ClientConn(m_port);
    m_awaiting.setup_and_listen();
    m_awaiting.handshake();
  }
}

void Server::main_loop() {

  while (true) {

    Request req;
    receive(req);
  }
}

Request Server::receive(Request &t_req) {

  /* single connection will block */
  if (m_connections.size() == SINGLE_CONNECTION) {
    return m_connections.at(SINGLE_CONNECTION).receive(t_req, Connection::is_blocking::True);
  };

  int index = 0;
  /* scan through connections to receive */
  for (auto &conn : m_connections) {
    if (conn.receive(t_req, Connection::is_blocking::True)) {
      return true;
      t_req.m_conn_index = index;
    }
    index++;
  }

  return false;
}

