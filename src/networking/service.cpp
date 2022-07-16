
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

// struct Service::Private {
//
//   static void will_quit(Service &t_self) {
//     if (t_self.m_req.m_command == commands::QUIT){
//       t_self.m_quit = true;
//     }
//   }
//
//   static void set_user(Service &t_self) {
//
//     if (t_self.m_logged_in)
//       return;
//
//     if (was_user_success_command(t_self)) {
//       t_self.m_user = t_self.m_req.m_argument;
//
//       if (was_annonymous_user(t_self)) {
//      //   t_self.m_logged_in = true;
//      //
//         /* responds logged in as anonynous */
//         t_self.m_req.m_reply = reply::r_230;
//         LOG_INFO("User %s is logged in.", t_self.m_user.c_str());
//       }
//     }
//   }
//
//   static void login(Service &t_self) {
//
//     if (t_self.m_logged_in)
//       return;
//
//     if (was_pass_success_command(t_self)) {
//       t_self.m_logged_in = true;
//       LOG_INFO("User %s is logged in.", t_self.m_user.c_str());
//     }
//   }
//
//   static void update_disk_state(Service &t_self) {
//     if (t_self.m_req.m_valid && disk_state_has_updated(t_self)) {
//       LOG_INFO("Path changed by user:  %s -> %s",
//                t_self.m_disk.m_user_path.c_str(),
//                t_self.m_req.m_disk.m_user_path.c_str());
//       t_self.m_disk = t_self.m_req.m_disk;
//     }
//   }
//
//   static void reset_request(Service &t_self) {
//     t_self.m_req = Request(t_self.m_disk, t_self.m_logged_in); // resets with
//     current disk state
//
//     if (!t_self.m_user.empty())
//       t_self.m_req.m_current_user = t_self.m_user;
//
//
//   }
//
//   static bool was_user_success_command(Service &t_self) {
//     return t_self.m_req.m_command == commands::name::USER &&
//            t_self.m_req.m_reply == reply::r_331;
//   }
//
//   static bool was_annonymous_user(Service &t_self) {
//     return t_self.m_user == controllers::Accounts::ANONYMOUS_USER;
//   }
//
//   static bool disk_state_has_updated(Service &t_self) {
//     return t_self.m_disk.m_user_path != t_self.m_req.m_disk.m_user_path &&
//            t_self.m_disk.m_system_path != t_self.m_req.m_disk.m_system_path;
//   }
//
//   static bool was_pass_success_command(Service &t_self) {
//     return t_self.m_req.m_command == commands::PASS &&
//            t_self.m_req.m_reply == reply::r_230;
//   }
// };

/* Constructor */

Service::Service(ClientConn &control_connection)
    : m_ctrlconn(control_connection), m_dataconn(0, active) {
   m_dataconn.set_socket_options();
}

void Service::work(Request &t_req) {

  parsing::Parser::parse(t_req);
  /* router delegates to a controller depending on command */
  routing::Router::route(t_req.m_command, t_req, m_dataconn);

  m_ctrlconn.login(t_req);

  m_ctrlconn.update_disk_state(t_req);
  m_ctrlconn.respond(t_req);

  data_connect(t_req);
  data_transfer(t_req);
}

void Service::data_connect(Request &t_req) {

  if (t_req.m_transfer == Request::connect) {
    m_dataconn.accept_connection(Connection::await::True);
  };
}

void Service::data_transfer(Request &t_req) {

  if (t_req.m_transfer == Request::send) {

     m_dataconn.transfer_send(t_req); // TODO many need error handling?

    /*  responds to client upon valid data transfer */
    auto req = Request(m_ctrlconn.is_logged_in());
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
    m_ctrlconn.respond(t_req);
  } else {
    t_req.m_reply = reply::r_451;
    m_ctrlconn.respond(t_req);
  }
}
