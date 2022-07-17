#ifndef SERVER_CONN_H
#define SERVER_CONN_H
#include "connection.hpp"
namespace networking {

class ServerConn: public Connection {
  using Connection::Connection;

  public:
    void        setup_and_listen();
    void        handshake();
    };

}

#endif
