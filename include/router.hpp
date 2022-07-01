
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "disk_manager.h"
#include "request.h"
#include "accounts.h"
#include "system_info.h"

namespace router {

typedef void (*Controller)(networking::Request &);

static std::map<commands::name, Controller> route {
    /* Accounts */
    {commands::name::USER, controllers::Accounts::verify_user},
    {commands::name::PASS, controllers::Accounts::verify_password},

    /* Disk */
    {commands::name::CWD, controllers::DiskManager::change_directory },
    {commands::name::CDUP, controllers::DiskManager::change_up_directory},
    {commands::name::PWD, controllers::DiskManager::print_working_directory},
    {commands::name::MKD, controllers::DiskManager::make_directory},
    {commands::name::RMD, controllers::DiskManager::remove_directory},
    {commands::name::RNFR, controllers::DiskManager::rename_from},
    {commands::name::RNTO, controllers::DiskManager::rename_to},

    /* System Info */
    {commands::name::SYST, controllers::SystemInfo::system_os},
    {commands::name::HELP, controllers::SystemInfo::help},


    /* Other */
    {commands::name::NOOP, [](networking::Request &t_req) { t_req.m_reply = networking::reply::r_200; }},
    {commands::name::QUIT, [](networking::Request &t_req) { t_req.m_reply = networking::reply::r_221; }}
};

} // namespace router

#endif
