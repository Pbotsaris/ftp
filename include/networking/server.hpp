#ifndef SERVER_H
#define SERVER_H
#include "server_conn.hpp"
#include "control_io.hpp"
#include "connection_poll.hpp"
#include "disk_manager.hpp"
#include "service.hpp"

using namespace networking;

class Server {

  typedef std::map<int, Service> Services;

  ServerConn               m_conn;
  Services                 m_services;
  ConnectionPoll           m_poll;

  public:
     Server(int t_port);
     
     bool connect();
     void main_loop();
 
  private:
     void        create_service();
     int         scan_sockets_for_requests();
     void        quit_service(int t_socket);
     
  };

#endif
