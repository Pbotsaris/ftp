#include "server_conn.hpp"
#include "accounts.hpp"
#include "logger.hpp"
#include "request.hpp"
#include "control_io.hpp"

using namespace networking;

void ServerConn::setup_and_listen() {
  set_socket_options();
  config_addr();
  bind_socket();
  socket_listen();
}

void ServerConn::handshake() {
  Request req(false);
  req.m_reply = reply::r_220;
  ControlIO io(get_connected_socket());
  io.respond(req);
}


