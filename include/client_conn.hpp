#ifndef CLIENT_CONN_H
#define CLIENT_CONN_H
#include "connection.hpp"

using namespace networking;

class ClientConn: public Connection {

  using Connection::Connection;

  private:
    bool        m_logged_in = false;
    std::string m_user;
    disk::Disk  m_disk;

  public:
    void        setup_and_listen();
    void        handshake();
    void        login(Request &t_req);
    void        update_disk_state(Request &t_req);
    bool        is_logged_in() const;
    std::string get_user() const;
    disk::Disk  get_disk() const; 


  private:
    bool        was_success_pass_command(const Request &t_req) const;
    bool        was_success_user_command(const Request &t_req) const;
    bool        was_annonymous_user() const;
    bool        disk_state_has_updated(const Request &t_req) const;
};


#endif
