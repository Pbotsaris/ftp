#include "connection_poll.hpp"
#include "logger.hpp"

ConnectionPoll::ConnectionPoll(){}

ConnectionPoll::ConnectionPoll(int socketfd, int t_timeout): m_timeout(t_timeout){

  m_poll[0].fd = socketfd;
  m_poll[0].events = 0;
  m_poll[0].events |= POLLIN;

}

bool ConnectionPoll::has_timeout(const char t_type[]){

  if(poll(m_poll, 1, m_timeout) == 0){
    LOG_INFO("%s timeout.", t_type);
    return true;
  }


  else return false;

}
