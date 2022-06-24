#include "../../include/service.h"

using namespace networking;

Service::Service(int t_port)
    : m_ctrlconn(t_port), m_dataconn(t_port, active), m_req(Request()), m_parser() {
  m_ctrlconn.set_socket_options();
  m_ctrlconn.config_addr();
}

void Service::setup() {
  m_ctrlconn.bind_socket();
  m_ctrlconn.socket_listen();
}

void Service::handshake() {
  m_ctrlconn.accept_connection();
  m_req.m_reply = reply::r_120;
  m_ctrlconn.respond(m_req);
}

void Service::main_loop() {
  while (!m_quit) {
    m_ctrlconn.receive(m_req);
    m_parser.parse(m_req);
    m_ctrlconn.respond(m_req);
  }
}
