#ifndef REQUEST_H
#define REQUEST_H
#include <vector>
#include "reply.hpp"

namespace request {

enum status{init, loaded, parsed, ready};

  struct Request {
    std::string m_raw;
    status m_status;
    reply::code m_reply;

   Request();
  };

}

#endif
