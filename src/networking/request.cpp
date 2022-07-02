#include "request.hpp"

using namespace networking;

Request::Request() : m_raw(), m_valid(true), m_reply(reply::nocode) {}

Request::Request(disk::Disk &t_disk) : m_raw(), m_valid(true), m_reply(reply::nocode), m_disk(t_disk) {}
