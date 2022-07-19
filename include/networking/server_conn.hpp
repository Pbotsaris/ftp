#ifndef SERVER_CONN_H
#define SERVER_CONN_H
#include "connection.hpp"
namespace networking {

class ServerConn: public Connection {
  using Connection::Connection;

  public:
    bool        setup_and_listen();
    bool        handshake();
    };

}

#endif
