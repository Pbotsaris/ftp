#ifndef CONNECTION_H
#define CONNECTION_H
#include "request.hpp"
#include <netinet/in.h> /* sockeaddr_in */
#include <queue>
#include <tuple>

namespace networking {

 enum conn_mode { active, passive };

  typedef  std::tuple<networking::ImageBuffer, std::uintmax_t>  DatafromClientTuple;


    struct TransferData{
    char *m_buffer;
    std::size_t m_length;
  };
  
  class Connection {

  private:
    int                   m_port;
    conn_mode             m_mode;
    int                   m_local_socket;
    int                   m_connected_socket;
    sockaddr_in           m_address;
  
  public:
    enum conn_type {none, image, ascii};
    enum await {True = true, False = false};

    Connection();
    Connection(int t_port, conn_mode t_mode = passive, conn_type t_type = ascii);
    ~Connection();
  
  private:
    conn_type             m_type; /* conn_type must be declared first */

  public:
    /* conn config and state */
    void                  config_addr();
    void                  config_addr(const std::string &t_ip);
    void                  set_socket_options();
    void                  bind_socket();
    void                  connect_socket();
    bool                  accept_connection(await await);
    void                  socket_listen();
    void                  reconnect();
    void                  make_passive_and_listen(int port);

    /* getters and setters */

    int                   get_socket_fd();
    int                   get_port();
    void                  set_port(int t_port);
    conn_mode             get_mode();
    void                  set_mode(conn_mode t_mode);
    conn_type             get_type();
    void                  set_type(conn_type t_type);

    /* used by control connections */
    bool                  receive(Request &t_req, await await);
    void                  respond(Request &t_req);

    /* used by data connections */

    void                  transfer_send(Request &t_req);
    DatafromClientTuple   transfer_receive(Request &t_req);

  private:
    void                  create_socket();
    void                  transfer_ascii(Request &t_req);
    void                  transfer_image(Request &t_req);

    /* this helper will consolidate memory chunks from queue into a consolidated  into a unique pointer */
    ImageBuffer           consolidate_data(std::queue<TransferData> t_data, std::uintmax_t t_total_length);

  };
}

#endif
