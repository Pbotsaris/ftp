#ifndef CONTROL_IO_H
#define CONTROL_IO_H
#include "connection.hpp"

namespace networking {

class ControlIO{
     int m_socket;

  public:
     ControlIO();
     ControlIO(int t_socket);

    bool                  receive(Request &t_req);
    bool                  respond(Request &t_req);
};
} // namespace networking


#endif
