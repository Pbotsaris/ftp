#ifndef SERVICE_H
#define SERVICE_H
#include "connection.hpp"
#include "disk.hpp"
#include "request.hpp"
#include "client_conn.hpp"

/*
 *   A Service corresponds single user interaction with the server and consists of two connections: 
 *     1. ctrconn to handle user issued commands.
 *     2. dataconn for data transfer.
 */

namespace networking {
  class Service {
    struct          Private;

    ClientConn      &m_ctrlconn;
    Connection      m_dataconn;
  
  public:
    Service(ClientConn &control_connection);
  
    void work(Request &t_req);

  private:
    void data_transfer(Request &t_req);
    void data_connect(Request &t_req);

    void data_transfer_respond(Request &t_req);

  };
}

#endif
