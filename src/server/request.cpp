#include "../../include/request.h"

request::Request::Request() : m_raw(), m_status(init), m_reply(reply::nocode) {}
