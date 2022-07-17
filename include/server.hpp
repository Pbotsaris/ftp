#ifndef SERVER_H
#define SERVER_H
#include "server_conn.hpp"
#include "control_io.hpp"
#include "connection_poll.hpp"
#include "disk_manager.hpp"
#include "service.hpp"

using namespace networking;

class Server {

  typedef std::vector<ServerConn> ConnectedClients;
  typedef std::vector<Service> Services;

  static const std::size_t SINGLE_CONNECTION;

  int                      m_port;
  ServerConn               m_conn;
  Services                 m_services;
  ConnectionPoll           m_connpoll;
  //std::vector<Service>     m_services;

  public:
     Server(int t_port);
     
     void new_connection();
     void main_loop();
 
  private:
     bool        receive(Request &t_req);
     bool        respond(Request &t_req);
     Service     create_service(Request &t_req);

     bool        single_service(Request &t_req);
     bool        multiple_services(Request &t_req);

     /* helpers */
     Service&    get_service(const Request &t_req) noexcept;
     Service&    get_service_at(const std::size_t t_index) noexcept;
     bool        service_exists(const Request &t_req) const noexcept;
     bool        service_exists(const std::size_t t_index) const noexcept;
     bool        prepare_request(Request &t_req);
};

#endif
