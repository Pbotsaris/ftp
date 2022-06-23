#ifndef SERVER_H
#define SERVER_H
#include "connection.h"

/*
 *   A Service corresponds single user interaction with the server and consists of two connections: 
 *     1. ctrconn to handle user issued commands.
 *     2. dataconn for data transfer.
 */

namespace networking {
  class Service {
  private:
    bool       m_quit;
    Connection m_ctrlconn;
    Connection m_dataconn;
    Request    m_req;
  
  public:
    Service(int t_port);
  
    void setup();
    void main_loop();
  };
}

#endif
