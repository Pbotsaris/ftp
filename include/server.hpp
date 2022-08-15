#ifndef SERVER_H
#define SERVER_H
#include "server_connection.hpp"
#include "control_io.hpp"
#include "connection_poll.hpp"
#include "disk_manager.hpp"
#include "service.hpp"
#include "thread_pool.hpp"

using namespace networking;
using namespace threads;

class Server {

  typedef std::map<int, Service> Services;
  typedef std::vector<std::future<int>> Futures;
  
  ServerConn               m_conn;
  Services                 m_services;
  ConnectionPoll           m_poll;
  ThreadPool               m_tpool;
  Futures                  m_futures;
  int                      m_no_activity;

  public:
     Server(int t_port);
     bool connect();
     void main_loop();
 
  private:
     void        create_service();
     void        quit_service(int t_socket);

     int         poll_futures();
     bool        is_future_ready(std::future<int> const& t_future);

     void        throttle();

};
 
#endif
