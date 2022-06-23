#ifndef SERVER_H
#define SERVER_H
#include "connection.h"

/*
 *  A Service corresponds single user interaction with the server and consists of two connections: 
 *     1. ctrconn to handle user issued commands.
 *     2. dataconn for data transfer.
 */

namespace networking {
  class Service {
  private:
    bool       quit;
    Connection m_ctrlconn;
    Connection m_dataconn;
  
  public:
    Service(int port);
  
    void setup();
    void main_loop();
  };
}

#endif
