
#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "disk_manager.hpp"
#include "request.hpp"
#include "accounts.hpp"
#include "system_info.hpp"
#include "data_manager.hpp"
#include "connection.hpp"

namespace routing {

typedef void (*ControlController)(networking::Request &);
typedef void (*DataController)(networking::Request &, networking::Connection &);

struct Router{
   static void route(const commands::name t_command, networking::Request &t_req, networking::Connection &t_dataconn);

  private:
   static std::map<commands::name, ControlController> m_ctrlrouter;
   static std::map<commands::name, DataController> m_datarouter;


   static bool is_control_router(const commands::name);
};

} // namespace router

#endif
