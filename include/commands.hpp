#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <string>

namespace commands {
  
  enum name {
    /* No arguments */
     CDUP = 0,     // cd ..
     QUIT = 1,     // disconnect (in ftp)
     PASV = 2,     // passive mode
//   ABOR = 3,     no support
     PWD  = 4,
     SYST = 5,     // returns OS
     NOOP = 6,

    /* With argument */
     USER = 9,
     PASS = 10,
     ACCT = 12,
     CWD  = 13,    // cd
//   SMNT = 14,    no support
//   REIN = 15,    no support
     PORT = 16,
     TYPE = 17,
//   STRU = 18,    no support, only single structure.
//   MODE = 19,    no support, stream only.
     RETR = 20,    // get
//   STOR = 21,    no support. Only unique files are created.
     STOU = 22,    // send
//   APPE = 23,    handled by client.
     REST = 24,    // restart
     RNFR = 25,    // rename
     RNTO = 26,    // rename
     DELE = 27,
     RMD  = 28,
     MKD  = 29,
     LIST = 30,
     NLST = 31,
     SITE = 32,
//   ALLO = 33,  no support
     HELP = 34,  // rhelp (in ftp)
     STAT = 35,  // rstatus (in ftp)
  };

  enum type {
    no_argument,
    with_argument,
  };

  static std::map<std::string, name > find {

    /* No arguments */
    { "CDUP", CDUP }, 
    { "QUIT", QUIT }, 
    { "PASV", PASV },
//  { "ABOR", ABOR },  no support
    { "PWD" , PWD  },
    { "SYST", SYST }, 
    { "NOOP", NOOP },
                      
    /* With argument */
    { "USER", USER },
    { "PASS", PASS },
    { "ACCT", ACCT },
    { "CWD ", CWD  },
//  { "SMNT", SMNT },  no support
//  { "REIN", REIN },  no support
    { "PORT", PORT },
    { "TYPE", TYPE },
//  { "STRU", STRU },  no support
//  { "MODE", MODE },  no support
    { "RETR", RETR },
    { "STOU", STOU },
//  { "STOR", STOR },  no support
//  { "APPE", APPE },  no support
    { "REST", REST },
    { "RNFR", RNFR },
    { "RNTO", RNTO },
    { "DELE", DELE },
    { "RMD" , RMD  },
    { "MKD" , MKD  },
    { "LIST", LIST },
    { "NLST", NLST },
    { "SITE", SITE },
//  { "ALLO", ALLO },  no support
    { "STAT", STAT }, 
    { "HELP", HELP }, 
  };


  struct Utils {
    static type get_type(name t_command) {

      if(t_command >= USER)
        return with_argument;

      else
        return no_argument;

    }

  };

}


#endif
