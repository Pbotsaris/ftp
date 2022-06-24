#ifndef REQUEST_H
#define REQUEST_H
#include <vector>
#include "reply.hpp"
#include"commands.hpp"


namespace networking {

enum status{init, loaded, parsed, ready};

  struct Request {
    std::string               m_raw;
    status                    m_status;
    bool                      m_valid;
    reply::code               m_reply;
    commands::name            m_command;
    std::string               m_argument;

   Request();
  };

}

#endif
