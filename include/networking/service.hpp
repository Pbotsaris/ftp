#ifndef SERVICE_H
#define SERVICE_H
#include "data_connection.hpp"
#include "control_io.hpp"
#include "disk.hpp"
#include "request.hpp"
#include "server_conn.hpp"

/*
 *   A Service corresponds single user interaction with the server and consists of two connections: 
 *     1. ctrconn to handle user issued commands.
 *     2. dataconn for data transfer.
 */

namespace networking {
  class Service {

    ControlIO       m_io;
    DataConnection  m_dataconn;
    bool            m_logged_in = false;
    std::string     m_user;
    disk::Disk      m_disk;

  
    public:
    Service(int t_connected_socket);
  
    void   work();

  private:
    Request     create_request();

    void        data_transfer(Request &t_req);
    void        data_connect(Request &t_req);
    void        data_transfer_respond(Request &t_req);

    void        login(Request &t_req);
    void        update_disk_state(Request &t_req);
    bool        was_success_pass_command(const Request &t_req) const;
    bool        was_success_user_command(const Request &t_req) const;
    bool        was_annonymous_user() const;
    bool        disk_state_has_updated(const Request &t_req) const;

  };
}
#endif

