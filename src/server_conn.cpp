#include "server_conn.hpp"
#include "accounts.hpp"
#include "logger.hpp"
#include "request.hpp"
#include "control_io.hpp"

using namespace networking;

bool ServerConn::setup_and_listen() {
  bool res = set_socket_options();
  config_addr();
  return res && bind_socket() && socket_listen();
}

bool ServerConn::handshake() {
  Request req(false);
  req.m_reply = reply::r_220;
  ControlIO io(get_connected_socket());
  return io.respond(req);
}


