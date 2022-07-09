#include "request.hpp"

using namespace networking;


Request::Request(bool t_logged_in)
    : m_raw(), m_valid(true), m_transfer(Request::none), m_reply(reply::nocode),
      m_logged_in(t_logged_in) {}

Request::Request(disk::Disk &t_disk, bool t_logged_in)
    : m_raw(), m_valid(true), m_transfer(Request::none), m_reply(reply::nocode),
      m_logged_in(t_logged_in), m_disk(t_disk) {}
