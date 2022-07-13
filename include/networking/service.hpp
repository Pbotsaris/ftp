#ifndef SERVICE_H
#define SERVICE_H
#include "connection.hpp"
#include "disk.hpp"
#include "request.hpp"

/*
 *   A Service corresponds single user interaction with the server and consists of two connections: 
 *     1. ctrconn to handle user issued commands.
 *     2. dataconn for data transfer.
 */

namespace networking {
  class Service {
    struct          Private;
    bool            m_quit;

    /* Disk Manager */
    disk::Disk      m_disk;  /* server disk paths */

    /* Connections */
    Connection      m_ctrlconn;
    Connection      m_dataconn;
    Request         m_req;


    /* User */
    bool            m_logged_in;
    std::string     m_user;


  
  public:
    Service(int t_control_port);
  
    void control_setup();
    void control_handshake();
    void control_loop();
    void control_disconnect();

  private:
    void data_transfer();
    void data_connect();

    void data_transfer_respond(Request &t_req);

  };
}

#endif
