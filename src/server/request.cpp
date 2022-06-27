#include "../../include/request.h"

using namespace networking;

Request::Request() : m_raw(), m_status(init), m_valid(true), m_reply(reply::nocode) {}
