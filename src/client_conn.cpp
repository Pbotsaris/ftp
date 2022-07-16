#include "client_conn.hpp" 
#include "request.hpp"

void ClientConn::setup_and_listen(){
  set_socket_options();
  config_addr();
  bind_socket();
  socket_listen();
}

void ClientConn::handshake(){
   Request req(m_logged_in);
   req.m_reply = reply::r_220;
   respond(req);
}

bool ClientConn::is_logged_in(){
  return m_logged_in;
}

