#include "../../include/service.h"

Service::Service(int t_port) : m_ctrlconn(t_port), m_dataconn(t_port, active) {
  m_ctrlconn.set_socket_options();
  m_ctrlconn.config_addr();
}

void Service::setup() {
  m_ctrlconn.bind_socket();
  m_ctrlconn.socket_listen();
}

void Service::main_loop() {
  while (!quit) {
    m_ctrlconn.accept_connection();
    m_ctrlconn.receive();
    m_ctrlconn.respond("Message Received!\n");
  }
}
