#ifndef REQUEST_H
#define REQUEST_H
#include <vector>
#include "reply.hpp"
#include "disk.h"
#include"commands.hpp"


namespace networking {

  struct Request {
    std::string               m_raw;
    bool                      m_valid;
    reply::code               m_reply;
    commands::name            m_command;
    std::string               m_argument;
    std::string               m_current_user;
    disk::Disk                m_disk;

   Request();
   Request(disk::Disk &t_disk);

  };

}

#endif
