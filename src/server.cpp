#include "server.hpp"
#include "reply.hpp"
#include "logger.hpp"


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

void Server::new_connection() {

  /* if an accept is successful setup another connection to await */
  if (m_awaiting.accept_connection(Connection::is_blocking::False)) {

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
    receive(req);
    load_req_with_conn_info(req);

    req.m_reply = reply::r_200;
    respond(req);
  }
}

Request Server::receive(Request &t_req) {

  /* single connection will block */
  if (m_connections.size() == SINGLE_CONNECTION) {
    t_req.m_conn_index = 0;
    return m_connections.at(0).receive(t_req, Connection::is_blocking::True);
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

void Server::respond(Request &t_req) {

  if(t_req.m_conn_index < m_connections.size()){
       m_connections.at(t_req.m_conn_index).respond(t_req);
  } else {
       LOG_ERROR("Could not respond. Requested connection is out of bounds.");
  }
}

void Server::load_req_with_conn_info(Request &t_req){
  if(t_req.m_conn_index < m_connections.size()){
      t_req.m_disk =  m_connections.at(t_req.m_conn_index).get_disk();
      t_req.m_logged_in = m_connections.at(t_req.m_conn_index).is_logged_in();
  } else {
       LOG_ERROR("Could not load request info. Requested connection is out of bounds.");
  }


}
