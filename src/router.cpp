#include "router.hpp"
#include <algorithm>

using namespace routing;

std::map<commands::name, ControlController> Router::m_ctrlrouter{
    /* Accounts */
    {commands::name::USER, controllers::Accounts::verify_user},
    {commands::name::PASS, controllers::Accounts::verify_password},

    /* Disk */
    {commands::name::CWD, controllers::DiskManager::change_directory},
    {commands::name::CDUP, controllers::DiskManager::change_up_directory},
    {commands::name::PWD, controllers::DiskManager::print_working_directory},
    {commands::name::MKD, controllers::DiskManager::make_directory},
    {commands::name::RMD, controllers::DiskManager::remove_directory},
    {commands::name::RNFR, controllers::DiskManager::rename_from},
    {commands::name::RNTO, controllers::DiskManager::rename_to},

    /* System Info */
    {commands::name::SYST, controllers::SystemInfo::system_os},
    {commands::name::HELP, controllers::SystemInfo::help},
    {commands::name::STAT, controllers::SystemInfo::status},

    /* Other */
    {commands::name::NOOP, [](networking::Request &t_req) { t_req.m_reply = networking::reply::r_200; }},
    {commands::name::QUIT, [](networking::Request &t_req) { t_req.m_reply = networking::reply::r_221; }}

};

std::map<commands::name, DataController> Router::m_datarouter{

    {commands::name::TYPE, controllers::DataManager::type},
    {commands::name::PORT, controllers::DataManager::port},
    {commands::name::RETR, controllers::DataManager::retrieve},
    {commands::name::LIST, controllers::DataManager::list},
};

void Router::route(const commands::name t_command, networking::Request &t_req,
                   networking::Connection &t_dataconn) {

  if (is_control_router(t_command))
    m_ctrlrouter[t_command](t_req);
  else
    m_datarouter[t_command](t_req, t_dataconn);
}

bool Router::is_control_router(const commands::name t_command) {

  std::vector<commands::name> control_commands;

  for (auto &command : m_ctrlrouter) {
    control_commands.push_back(command.first);
  }

  return std::any_of(control_commands.begin(), control_commands.end(),
                     [t_command](commands::name cmd) { return cmd == t_command; });
}