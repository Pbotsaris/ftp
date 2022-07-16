#include "client_conn.hpp"
#include "accounts.hpp"
#include "logger.hpp"
#include "request.hpp"

void ClientConn::setup_and_listen() {
  set_socket_options();
  config_addr();
  bind_socket();
  socket_listen();
}

void ClientConn::handshake() {
  Request req(m_logged_in);
  respond(req);
  req.m_reply = reply::r_220;
}

void ClientConn::login(Request &t_req) {

  /* set user */
  if (!m_logged_in && was_success_user_command(t_req)) {
    m_user = std::move(t_req.m_current_user);
    /* change response for ANONYMOUS_USER as pass is not required.  */
    if (was_annonymous_user(t_req)) {
      t_req.m_reply = reply::r_230;
      LOG_INFO("User %s is logged in.", m_user.c_str());
    }
  }

  /* password */
  if (!m_logged_in && was_success_pass_command(t_req)) {
    m_logged_in = true;
    LOG_INFO("User %s is logged in.", m_user.c_str());
  }
}
void ClientConn::update_disk_state(Request &t_req) {

  if (t_req.m_valid && disk_state_has_updated(t_req)) {
    LOG_INFO("Path changed by user:  %s -> %s", m_disk.m_user_path.c_str(),

             t_req.m_disk.m_user_path.c_str());
    m_disk = t_req.m_disk;
  }
}

bool ClientConn::is_logged_in() const { return m_logged_in; }

disk::Disk ClientConn::get_disk() const { return m_disk; }

/* PRIVATE */

bool ClientConn::was_success_pass_command(const Request &t_req) const {
  return t_req.m_command == commands::PASS && t_req.m_reply == reply::r_230;
}

bool ClientConn::was_success_user_command(const Request &t_req) const {
  return t_req.m_command == commands::name::USER &&
         t_req.m_reply == reply::r_331;
}

bool ClientConn::was_annonymous_user(const Request &t_req) const {
  return m_user == controllers::Accounts::ANONYMOUS_USER;
}

bool ClientConn::disk_state_has_updated(const Request &t_req) const {
  return m_disk.m_user_path != t_req.m_disk.m_user_path &&
         m_disk.m_system_path != t_req.m_disk.m_system_path;
}
