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

#include "request.hpp"
#include "logger.hpp"

namespace networking {
  enum conn_mode { active, passive };

  enum conn_type {none, image, acii};
  
  class Connection {
  private:
    int         m_port;
    conn_mode   m_mode;
    conn_type   m_type;
    int         m_local_socket;
    int         m_connected_socket;
    sockaddr_in m_address;
  
  public:
    Connection();
    Connection(int t_port, conn_mode t_mode = passive, conn_type t_type = none);
    ~Connection();
  
    void        config_addr();
    void        config_addr(const std::string &t_ip);
    void        set_socket_options();
    void        bind_socket();
    void        connect_socket();
    void        accept_connection();
    void        socket_listen();
    void        receive(Request &t_req);
    void        respond(Request &t_req);
    void        transfer(Request &t_req);
    int         get_port();
    void        set_port(int t_port);
    void        set_mode(conn_mode t_mode);
    conn_type   get_type();
    void        set_type(conn_type t_type);

  private:
    void        create_socket();
  };
}

#endif
