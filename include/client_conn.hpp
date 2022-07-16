#ifndef CLIENT_CONN_H
#define CLIENT_CONN_H
#include "connection.hpp"

using namespace networking;

class ClientConn: public Connection {

  using Connection::Connection;

  private:
    bool  m_logged_in = false;

  public:
    void setup_and_listen();
    void handshake();
    bool is_logged_in();

};


#endif
