#include "connection_poll.hpp"
#include "logger.hpp"

ConnectionPoll::ConnectionPoll() {}

ConnectionPoll::ConnectionPoll(int t_socket_fd, int t_timeout)
    : m_timeout(t_timeout), m_poll_index(1) {

  /* init poll to locket socket */
  m_poll[m_local_socket].fd = t_socket_fd;
  m_poll[m_local_socket].events = 0;
  m_poll[m_local_socket].events |= POLLIN;

}

bool ConnectionPoll::add_socket(int t_socket_fd) {

  /* add connected sockets to polling system */

  if (m_poll_index >= m_max) {
    return false;
  }

  m_poll[m_poll_index].fd = t_socket_fd;
  m_poll[m_poll_index].events = 0;
  m_poll[m_poll_index].events |= POLLIN;
  m_poll_index++;

  return true;
}

bool ConnectionPoll::accept_awaits() {

    if (poll(m_poll, m_poll_index + 1, m_timeout) > 0) {
      return true;
  }

  return false;
}

int ConnectionPoll::select_socket_with_events() {
  for (int i = 1; i < m_max; i++) {
    if (poll(m_poll, i + 1, m_timeout) > 0) {

      if (m_poll[0].revents & POLLIN) {
        return m_poll[0].fd;
      }
    }
  }

  return -1;
}

void ConnectionPoll::set_timeout(int t_timeout) { m_timeout = t_timeout; }

bool ConnectionPoll::has_timeout(const char t_type[]) {

  if (poll(m_poll, 1, m_timeout) == 0) {
    LOG_INFO("%s timeout.", t_type);
    return true;
  }

  else
    return false;
}
