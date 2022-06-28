
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "disk_manager.h"
#include "request.h"
#include "users.h"

namespace router {

typedef void (*Controller)(networking::Request &);

static std::map<commands::name, Controller> route {
    {commands::name::USER, controllers::Accounts::verify_user},
    {commands::name::PASS, controllers::Accounts::verify_password},
    {commands::name::CWD, controllers::DiskManager::change_directory },
    {commands::name::QUIT, [](networking::Request &t_req) { t_req.m_reply = networking::reply::r_221; }}
};

} // namespace router

#endif
