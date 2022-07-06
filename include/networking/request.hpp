#ifndef REQUEST_H
#define REQUEST_H
#include <vector>
#include "reply.hpp"
#include "disk.hpp"
#include"commands.hpp"

namespace networking {

  struct Data {
    std::string m_ascii;
    std:: string m_image;
  };

  struct Request {
    std::string               m_raw;                   /* raw message from client */
    bool                      m_valid;                 /* parsing was valid ? */
    bool                      m_isdata;                /* weather service should transfer the data,. */
    reply::code               m_reply;                 /* reply code  */
    std::string               m_reply_msg;             /* reply message if any  */
    commands::name            m_command;               /* command name. See commands.hpp */
    std::string               m_argument;              /* command argument if any */
    std::string               m_current_user;          /* current of the server generating the request */
    bool                      m_logged_in;             /* was user logged in when request was generated */
    disk::Disk                m_disk;                  /* path/disk information of the server */
    Data                      m_data;

   Request(bool t_logged_in = false);
   Request(disk::Disk &t_disk, bool t_logged_in = false);

  };
}

#endif
