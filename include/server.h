#ifndef SERVER_H
#define SERVER_H
#include <cstdio>
#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct Connection {
  public:
    std::string req;
    std::string res;
};

typedef void(*ServerCallback)(Connection*);

class Server {

private:
  int         m_port;
  int         m_queue_size;      /* maximum size of the backlog queue is 5 */
  int         m_socket;
  int         m_connected_socket;
  sockaddr_in m_server_addr;
  sockaddr_in m_client_addr;

public:
  Server(int t_port, int t_queue_size);

  void config_addr();
  void config_addr(std::string &t_ip);
  void set_socket_options();
  void bind_socket();
  void slisten();
  void receive();
};

#endif
