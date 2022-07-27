#ifndef CONNECTION_POLL_H
#define CONNECTION_POLL_H
#include <poll.h>
#include <iostream>

#define MAX_CONNECTIONS 10
#define OFFSET 1

/* m_poll[0] is the local socket while the m_poll[n] where n > 0 is the connected sockets */

class ConnectionPoll {

  const std::size_t  m_max = MAX_CONNECTIONS;
  const std::size_t  m_local_socket = 0;
  struct pollfd      m_poll[MAX_CONNECTIONS];
  int                m_timeout;
  std::size_t        m_poll_index = OFFSET;

  public:
  ConnectionPoll();
  ConnectionPoll(int t_socket_fd, int t_timeout = 100);

  bool  add_socket(int t_socket_fd);
  void  remove_socket(int t_socket_fd);
  bool  accept_awaits();
  int   select_socket_with_events();
  void  close_all();

  void  reposition_poll();
  void  count_poll_index();

  int   get_max_connections();

  void  set_timeout(int t_timeout);
  bool  has_timeout(const char t_buffer[]);

};

#endif // !CONNECTION_POOL_H
