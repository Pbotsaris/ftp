#ifndef SERVICE_H
#define SERVICE_H
#include "data_connection.hpp"
#include "control_io.hpp"
#include "disk.hpp"
#include "request.hpp"
#include "server_conn.hpp"

namespace networking {

  class Service {

    ControlIO       m_io;
    DataConnection  m_dataconn;
    bool            m_logged_in = false;
    std::string     m_user;
    disk::Disk      m_disk;

  
    public:
    Service(int t_connected_socket);
  
    bool        work();

  private:
    Request     create_request();

    /* Data Connection */
    void        data_transfer(Request &t_req);
    void        data_connect(Request &t_req);
    void        data_transfer_respond(Request &t_req);

    /* Updated service state */
    void        login(Request &t_req);
    void        update_disk_state(Request &t_req);

    /* Conditionals */
    bool        was_success_pass_command(const Request &t_req) const;
    bool        was_success_user_command(const Request &t_req) const;
    bool        was_annonymous_user() const;
    bool        disk_state_has_updated(const Request &t_req) const;
    bool        was_quit(const Request &t_req) const;
  };
}
#endif

