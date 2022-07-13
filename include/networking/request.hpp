#ifndef REQUEST_H
#define REQUEST_H
#include <cstdint>
#include <vector>
#include <memory>
#include <tuple>

#include "reply.hpp"
#include "disk.hpp"
#include"commands.hpp"

namespace networking {

  typedef std::unique_ptr<char[]> ImageBuffer;

  struct Data {
    std::string m_ascii;
    ImageBuffer m_image; /* buffer of bytes */
    std::size_t m_image_size;
  };

  struct Request {
    enum data_transfer {none, send, receive, connect };

    std::string               m_raw;                   /* raw message from client */
    bool                      m_valid;                 /* was a request modifier valid ? */
    data_transfer             m_transfer;              /* wether service should send or receive via data connection */
    reply::code               m_reply;                 /* reply code  */
    std::string               m_reply_msg;             /* reply message if any  */
    commands::name            m_command;               /* command name. See commands.hpp */
    std::string               m_argument;              /* command argument if any */
    std::string               m_current_user;          /* the user who generates the request */
    bool                      m_logged_in;             /* was user logged in when request was generated */
    disk::Disk                m_disk;                  /* Current path/disk state */
    Data                      m_data;                  /* Data to be transfered via data connection if any */
    int                       m_dataport;               /* The port for passive data connections */

   Request(bool t_logged_in = false);
   Request(disk::Disk &t_disk,  bool t_logged_in = false);

  };
}

#endif
