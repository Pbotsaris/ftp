#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include "request.h"

namespace controllers {

  struct SystemInfo {
    static void        system_os(networking::Request &t_req);
    static void        help(networking::Request &t_req);
    static void        status(networking::Request &t_req);
    static void        noop(networking::Request &t_req);

    private:
    static void        get_help(networking::Request &t_req);
    static void        list_help(networking::Request &t_req);

    static std::string get_os_name();

    static void        to_upper(std::string &t_str);


  };

}


#endif
