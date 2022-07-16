#ifndef SERVER_H
#define SERVER_H
#include "client_conn.hpp"
#include "disk_manager.hpp"
#include "service.hpp"

using namespace networking;
class Server {

  typedef std::vector<ClientConn> ConnectedClients;
  static const std::size_t SINGLE_CONNECTION;

  int                      m_port;
  ClientConn               m_awaiting;
  ConnectedClients         m_connections;
  //std::vector<Service>     m_services;

  public:
     Server(int t_port);
     
     void new_connection();
     void main_loop();
 
  private:
     bool        receive(Request &t_req);
     bool        respond(Request &t_req);
     Service     create_service(Request &t_req);

     bool        single_connection(Request &t_req);
     bool        multiple_connections(Request &t_req);

     /* helpers */
     ClientConn& get_connection(const Request &t_req) noexcept;
     ClientConn& get_connection_at(const std::size_t t_index) noexcept;
     bool        conn_exist(const Request &t_req) const noexcept;
     bool        conn_exist(const std::size_t t_index) const noexcept;
     bool        load_req_with_conn_info(Request &t_req) const;
};

#endif
