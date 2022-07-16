#ifndef CONNECTION_POLL_H
#define CONNECTION_POLL_H

#include <poll.h>
#include <string>


class ConnectionPoll {

  struct pollfd m_poll[1];
  int m_timeout;

  public:
  ConnectionPoll(int socketfd, int t_timeout = 500);

  bool has_timeout(const char t_buffer[]);


};

#endif // !CONNECTION_POOL_H
