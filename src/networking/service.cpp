
#include "service.hpp"
#include "connection.hpp"
#include "disk_manager.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "request.hpp"
#include "router.hpp"
#include "utils_file.hpp"

#include <exception>
#include <map>

using namespace networking;

/* Constructor */

Service::Service(int t_connected_socket)
    : m_io(t_connected_socket),
      m_dataconn(0, active) {
     // m_ctrlconn.setup_and_listen();    
      m_dataconn.set_socket_options();
      m_dataconn.set_type(DataConnection::ascii);
      controllers::DiskManager::init(m_disk);
}


void Service::work() {

  /* create request with with this service current state (such as disk, user etc) */
  Request req = create_request();

  /* receive from socket */
  m_io.receive(req);

  parsing::Parser::parse(req);

  /* router delegates to a controller depending on command */
  routing::Router::route(req.m_command, req, m_dataconn);

  /* check if user is logged in and update (if applicable) */
  login(req);
  /* update service state has changed */
  update_disk_state(req);

  m_io.respond(req);

  data_connect(req);
  data_transfer(req);
}


Request Service::create_request(){
    Request req = Request();
    req.m_disk = m_disk;
    req.m_logged_in = m_logged_in;
    req.m_current_user = m_user;

    return req;
}


void Service::data_connect(Request &t_req) {

  if (t_req.m_transfer == Request::connect) {
    m_dataconn.accept_connection();
  };
}

void Service::data_transfer(Request &t_req) {

  if (t_req.m_transfer == Request::send) {
     m_dataconn.transfer_send(t_req); // TODO many need error handling?

    /*  responds to client upon valid data transfer */
    auto req = Request(m_logged_in);
    req.m_valid = true;
    data_transfer_respond(req);
  }

  if (t_req.m_transfer == Request::receive) {

    networking::DatafromClientTuple data = m_dataconn.transfer_receive(t_req);

    try {
      utils::FileHelpers::write_to_disk(t_req, data);
    } catch (std::exception &err) {
      LOG_ERROR(err.what());
      t_req.m_valid = false;
    }

    data_transfer_respond(t_req);
  }
}

void Service::data_transfer_respond(Request &t_req) {

  if (t_req.m_valid) {
    t_req.m_reply = reply::r_226;
    m_io.respond(t_req);
  } else {
    t_req.m_reply = reply::r_451;
    m_io.respond(t_req);
  }
}


void Service::login(Request &t_req) {
  /* set user */
  if (!m_logged_in && was_success_user_command(t_req)) {
     m_user = std::move(t_req.m_argument);
    if (was_annonymous_user()) {
      /* change response for ANONYMOUS_USER as pass is not required.  */
      m_logged_in   = true;
      t_req.m_reply = reply::r_230;
      LOG_INFO("User %s is logged in.", m_user.c_str());
    }
    return;
  }

  /* password */
  if (!m_logged_in && was_success_pass_command(t_req)) {
    m_logged_in = true;
    t_req.m_reply = reply::r_230;
    LOG_INFO("User %s is logged in.", m_user.c_str());
  }
}

void Service::update_disk_state(Request &t_req) {

  if (t_req.m_valid && disk_state_has_updated(t_req)) {
    LOG_INFO("Path changed by user:  %s -> %s", m_disk.m_user_path.c_str(),

             t_req.m_disk.m_user_path.c_str());
    m_disk = t_req.m_disk;
  }
}

bool Service::was_success_pass_command(const Request &t_req) const {
  return t_req.m_command == commands::PASS && t_req.m_reply == reply::r_230;
}

bool Service::was_success_user_command(const Request &t_req) const {
  return t_req.m_command == commands::name::USER &&
         t_req.m_reply == reply::r_331;
}

bool Service::was_annonymous_user() const {
  return m_user == controllers::Accounts::ANONYMOUS_USER;
}

bool Service::disk_state_has_updated(const Request &t_req) const {
  return m_disk.m_user_path != t_req.m_disk.m_user_path &&
         m_disk.m_system_path != t_req.m_disk.m_system_path;
}
