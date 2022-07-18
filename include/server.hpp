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
  ConnectionPoll           m_poll;

  public:
     Server(int t_port);
     
     bool connect();
     void main_loop();
 
  private:
     Service     create_service(Request &t_req);
  };

#endif
