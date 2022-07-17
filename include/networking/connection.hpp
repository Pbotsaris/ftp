#ifndef CONNECTION_H
#define CONNECTION_H
#include "request.hpp"
#include <netinet/in.h> /* sockeaddr_in */
#include <queue>
#include <tuple>

namespace networking {

 enum conn_mode { active, passive };
  
  class Connection {

  private:
    int                   m_port;
    conn_mode             m_mode;
    int                   m_local_socket;
    int                   m_connected_socket;
    sockaddr_in           m_address;
  
  public:

    Connection();
    Connection(int t_port, conn_mode t_mode = passive);
    ~Connection();

    /* conn config and state */
    void                  config_addr();
    void                  config_addr(const std::string &t_ip);
    void                  set_socket_options();
    void                  bind_socket();
    void                  connect_socket();
    int                   accept_connection();
    void                  make_passive_and_listen(int port);
    void                  socket_listen();
    void                  reconnect();
    void                  create_socket();

    /* getters and setters */
    int                   get_local_socket();
    int                   get_connected_socket();
    int                   get_port();
    void                  set_port(int t_port);
    conn_mode             get_mode();
    void                  set_mode(conn_mode t_mode);


  };
}

#endif
