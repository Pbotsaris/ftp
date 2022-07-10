
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

struct Service::Private {

  static void will_quit(Service &t_self) {
    if (t_self.m_req.m_command == commands::QUIT)
      t_self.m_quit = true;
  }

  static void set_user(Service &t_self) {

    if (t_self.m_logged_in)
      return;

    if (was_user_success_command(t_self)) {
      t_self.m_user = t_self.m_req.m_argument;

      if (was_annonymous_user(t_self)) {
        t_self.m_logged_in = true;
        /* responds logged in as anonynous */
        t_self.m_req.m_reply = reply::r_230;
        LOG_INFO("User %s is logged in.", t_self.m_user.c_str());
      }
    }
  }

  static void login(Service &t_self) {

    if (t_self.m_logged_in)
      return;

    if (was_pass_success_command(t_self)) {
      t_self.m_logged_in = true;
      LOG_INFO("User %s is logged in.", t_self.m_user.c_str());
    }
  }

  static void update_disk_state(Service &t_self) {
    if (t_self.m_req.m_valid && disk_state_has_updated(t_self)) {
      LOG_INFO("Path changed by user:  %s -> %s",
               t_self.m_disk.m_user_path.c_str(),
               t_self.m_req.m_disk.m_user_path.c_str());
      t_self.m_disk = t_self.m_req.m_disk;
    }
  }

  static void reset_request(Service &t_self) {
    t_self.m_req = Request(
        t_self.m_disk, t_self.m_logged_in); // resets with current disk state

    if (!t_self.m_user.empty())
      t_self.m_req.m_current_user = t_self.m_user;
  }

  static bool was_user_success_command(Service &t_self) {
    return t_self.m_req.m_command == commands::name::USER &&
           t_self.m_req.m_reply == reply::r_331;
  }

  static bool was_annonymous_user(Service &t_self) {
    return t_self.m_user == controllers::Accounts::ANONYMOUS_USER;
  }

  static bool disk_state_has_updated(Service &t_self) {
    return t_self.m_disk.m_user_path != t_self.m_req.m_disk.m_user_path &&
           t_self.m_disk.m_system_path != t_self.m_req.m_disk.m_system_path;
  }

  static bool was_pass_success_command(Service &t_self) {
    return t_self.m_req.m_command == commands::PASS &&
           t_self.m_req.m_reply == reply::r_230;
  }
};

/* Constructor */

Service::Service(int t_port)
    : m_ctrlconn(t_port), m_dataconn(t_port++, active), m_logged_in(false),
      m_disk() {

  controllers::DiskManager::init(m_disk);
  m_req = Request(m_disk, m_logged_in);
  m_ctrlconn.set_socket_options();
  m_ctrlconn.config_addr();

  /* data options */
  m_dataconn.set_socket_options();
}

/* Public */

void Service::control_setup() {
  m_ctrlconn.bind_socket();
  m_ctrlconn.socket_listen();
}

void Service::control_handshake() {

  m_ctrlconn.accept_connection();
  m_req.m_reply = reply::r_220;
  m_ctrlconn.respond(m_req);

  LOG_DEBUG("Handshake complete. respond with 220.");

  // m_req.m_reply = reply::r_120;
  // m_ctrlconn.respond(m_req);
}

void Service::control_loop() {
  while (!m_quit) {

    /* msg from client */
    m_ctrlconn.receive(m_req);

    /* Parses raw client command */
    parsing::Parser::parse(m_req);

    /* router delegates to a controller depending on command */
    routing::Router::route(m_req.m_command, m_req, m_dataconn);

    /* Sets user and login only on USER and PASS commands*/
    Private::set_user(*this);
    Private::login(*this);

    /* when changes occur */
    Private::update_disk_state(*this);

    Private::will_quit(*this);

    /* to client */
    m_ctrlconn.respond(m_req);

    /* transfer data if any. This may trigger another response via control */
    data_transfer();

    Private::reset_request(*this);
  }

  control_disconnect();
}

void Service::control_disconnect() {

  int ctrl_port = m_ctrlconn.get_port();
  m_ctrlconn = Connection(ctrl_port); // restablish new connection
}

void Service::data_transfer() {
  if (m_req.m_transfer == Request::send) {
    m_dataconn.transfer_send(m_req);

    /*  responds to client upon valid data transfer */
    auto req = Request(m_logged_in);
    data_transfer_respond(req);
  }

  if (m_req.m_transfer == Request::receive) {

    networking::DatafromClientTuple data = m_dataconn.transfer_receive(m_req);

    try {
      utils::FileHelpers::write_to_disk(m_req, data);
    } catch (std::exception &err) {
      LOG_ERROR(err.what());
      m_req.m_valid = false;
    }

    data_transfer_respond(m_req);
  }
}

void Service::data_transfer_respond(Request &t_req) {

  if (m_req.m_valid) {
    t_req.m_reply = reply::r_226;
    m_ctrlconn.respond(t_req);
  } else {
    t_req.m_reply = reply::r_451;
    m_ctrlconn.respond(t_req);
  }
}
