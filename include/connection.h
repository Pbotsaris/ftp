#ifndef CONNECTION_H
#define CONNECTION_H

#include <exception>
#include <iostream>
#include <string>

/* C headers */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "request.h"
#include "logger.hpp"

namespace networking {
  enum conn_mode { active, passive };
  
  class Connection {
  private:
    int         m_port;
    conn_mode   m_mode;
    int         m_local_socket;
    int         m_connected_socket;
    sockaddr_in m_address;
  
  public:
    Connection(int t_port, conn_mode t_mode = passive);
    ~Connection();
  
    void config_addr();
    void config_addr(const std::string &t_ip);
    void set_socket_options();
    void bind_socket();
    void connect_socket();
    void accept_connection();
    void socket_listen();
    void receive(Request &t_req);
    void respond(Request &t_req);
  };
}

#endif
