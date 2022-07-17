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

Server::Server(int t_port)
    : m_port(t_port), m_conn(t_port),
      m_connpoll(m_conn.get_local_socket()) {
      m_conn.setup_and_listen();
      }

void Server::new_connection() {
  int connected_socket = m_conn.accept_connection();
  m_conn.handshake();
  m_services.push_back(Service(connected_socket));
}

void Server::main_loop() {

  while (true) {
      m_services.at(0).work();
  }
}
































// Server::Server(int t_port) :
//   m_port(t_port),
//   m_ctrlconn(t_port),
//   m_awaiting_service(m_ctrlconn),
//   m_connpoll(m_awaiting_service.get_control_socket())
//{ };
//
// void Server::new_connection() {
//   /* if an accept is successful setup another connection to await */
//   if (m_connpoll.accept_awaits()) {
//
//     /* move connected connection to the m_services vector */
//     m_awaiting_service.accept_control_connection();
//     m_awaiting_service.control_handshake();
//    // m_services.push_back(std::move(m_awaiting_service));
//
//   }
// }
//
// void Server::main_loop() {
//
//   while (true) {
//     Request req;
//     bool result = receive(req);
//
//     if (result && prepare_request(req)) {
//           get_service(req).work(req);
//     } else {
//       LOG_ERROR("Could not start service.");
//       break;
//     }
//   }
// }
//
// bool Server::receive(Request &t_req) {
//
//   if (m_services.size() == SINGLE_CONNECTION) {
//     return single_service(t_req);
//   };
//
//   return multiple_services(t_req);
// }
//
// bool Server::respond(Request &t_req) {
//
//   if (service_exists(t_req)) {
//      get_service(t_req).control_respond(t_req);
//     return true;
//   } else {
//     LOG_ERROR("Could not respond. Requested connection is out of bounds.");
//     return false;
//   }
// }
//
///****** Helpers *****/
//
// bool Server::single_service(Request &t_req) {
//  t_req.m_conn_index = 0;
//
//  /* single connection will block */
//  return get_service_at(0).control_receive(t_req);
//}
//
// bool Server::multiple_services(Request &t_req) {
//  int index = 0;
//  /* scan through connections to receive */
//  for (auto &conn : m_services) {
//    if (conn.control_receive(t_req)) {
//      t_req.m_conn_index = index;
//
//      return true;
//    }
//    index++;
//  }
//  return false;
//}
//
// bool Server::prepare_request(Request &t_req)  {
//
//  if (service_exists(t_req)) {
//    m_services.at(t_req.m_conn_index).load_request_service_info(t_req);
//    return true;
//  } else {
//    LOG_ERROR( "Could not load request info. Requested connection is out of
//    bounds."); return false;
//  }
//}
//
// Service &Server::get_service(const Request &t_req) noexcept {
//  return m_services.at(t_req.m_conn_index);
//}
//
// Service &Server::get_service_at(const std::size_t t_index) noexcept {
//  return m_services.at(t_index);
//}
//
// bool Server::service_exists(const Request &t_req) const noexcept {
//  return t_req.m_conn_index < m_services.size();
//}
//
// bool Server::service_exists(const std::size_t t_index) const noexcept {
//  return t_index < m_services.size();
//}
