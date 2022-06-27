#include "../../include/service.h"

using namespace networking;

struct Service::Private {

  static void will_quit(Service &t_self){
    if(t_self.m_req.m_command == commands::QUIT)
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

  static bool was_user_success_command(Service &t_self) {
    return t_self.m_req.m_command == commands::name::USER &&
           t_self.m_req.m_reply == reply::r_331;
  }

  static bool was_annonymous_user(Service &t_self) {
    return t_self.m_user == controllers::Accounts::ANONYMOUS_USER;
  }

  static bool was_pass_success_command(Service &t_self) {
    return t_self.m_req.m_command == commands::PASS &&
           t_self.m_req.m_reply == reply::r_230;
  }

  static void reset_request(Service &t_self) {
    t_self.m_req = Request();

    if (!t_self.m_user.empty())
      t_self.m_req.m_current_user = t_self.m_user;
  }
};

Service::Service(int t_port)
    : m_ctrlconn(t_port), m_dataconn(t_port++, active), m_req(Request()),
      m_logged_in(false) {
  m_ctrlconn.set_socket_options();
  m_ctrlconn.config_addr();
}

void Service::control_setup() {
  m_ctrlconn.bind_socket();
  m_ctrlconn.socket_listen();
}

void Service::control_handshake() {

  m_ctrlconn.accept_connection();
  m_req.m_reply = reply::r_120;
  m_ctrlconn.respond(m_req);
}

void Service::control_loop() {
  while (!m_quit) {

    m_ctrlconn.receive(m_req);

    /* Parses raw client command */
    parsing::Parser::parse(m_req);

    /* router delegates to a controller depending on command */
      router::route[m_req.m_command](m_req);

    /* Sets user and login on USER and PASS commands*/
    Private::set_user(*this);
    Private::login(*this);

    Private::will_quit(*this); // ?

    m_ctrlconn.respond(m_req);

    Private::reset_request(*this);
  }

   control_disconnect();
}

void Service::control_disconnect() {

  int ctrl_port = m_ctrlconn.get_port();
  m_ctrlconn = Connection(ctrl_port); // restablish new connection
}
