
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "request.h"
#include "users.h"

namespace router {

  typedef void (*Controller)(networking::Request&);

  static std::map<commands::name, Controller> route {
   { commands::name::USER, controllers::Accounts::verify_user},
   { commands::name::PASS, controllers::Accounts::verify_password},
    {commands::name::QUIT, [](networking::Request& t_req){ t_req.m_reply = networking::reply::r_221;}}
  };





}

#endif
