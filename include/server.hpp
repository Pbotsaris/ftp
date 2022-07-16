#ifndef SERVER_H
#define SERVER_H
#include "client_conn.hpp"
#include "disk_manager.hpp"

using namespace networking;
class Server {

  typedef std::vector<ClientConn> ConnectedClients;
  static const int SINGLE_CONNECTION;

  int                      m_port;
  ClientConn               m_awaiting;
  ConnectedClients         m_connections;
 // std::vector<Service>     m_services;

  public:
     Server(int t_port);
     
     void new_connection();
     void main_loop();
 

  private:
     Request receive(Request &t_req);
     void    respond(Request &t_req);
     void    load_req_with_conn_info(Request &t_req);

};

#endif
