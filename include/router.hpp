
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "request.h"
#include "users.h"

namespace router {

  typedef void (*Controller)(networking::Request&);

  static std::map<commands::name, Controller> route {
   { commands::name::USER, controllers::Accounts::verify_user},
   { commands::name::PASS, controllers::Accounts::verify_password},
  };





}

#endif
