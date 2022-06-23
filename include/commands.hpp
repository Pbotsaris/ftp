#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <string>

namespace commands {
  
  enum name {
    /* No arguments */
     CDUP = 0,
     QUIT = 2, 
     PASV = 3, 
     STOU = 4,
     ABOR = 5,
     PWD  = 6,
     SYST = 7,
     NOOP = 8,

    /* Single argument */
     USER = 100,
     PASS = 101,
     ACCT = 102,
     CWD  = 103,
     SMNT = 104,
     REIN = 105,
     PORT = 106,
     TYPE = 107,
     STRU = 108,
     MODE = 109,
     RETR = 110,
     STOR = 111,
     APPE = 112,
     REST = 113,
     RNFR = 114,
     RNTO = 115,
     DELE = 116,
     RMD  = 117,
     MKD  = 118,
     LIST = 119,
     NLST = 120,
     SITE = 121,

    /* List argument */
     ALLO = 1000,
     STAT = 1001,
     HELP = 1002,
  };

  enum type {
    no_argument,
    single_argument,
    list_argument,
  };

  static std::map<std::string, name > find {

    /* No arguments */
    { "CDUP", CDUP }, 
    { "QUIT", QUIT }, 
    { "PASV", PASV },
    { "STOU", STOU },
    { "ABOR", ABOR },
    { "PWD" , PWD  },
    { "SYST", SYST }, 
    { "NOOP", NOOP },
                      
    /* single argument */
    { "USER", USER },
    { "PASS", PASS },
    { "ACCT", ACCT },
    { "CWD ", CWD  },
    { "SMNT", SMNT },
    { "REIN", REIN },
    { "PORT", PORT },
    { "TYPE", TYPE },
    { "STRU", STRU },
    { "MODE", MODE },
    { "RETR", RETR },
    { "STOR", STOR },
    { "APPE", APPE },
    { "REST", REST },
    { "RNFR", RNFR },
    { "RNTO", RNTO },
    { "DELE", DELE },
    { "RMD" , RMD  },
    { "MKD" , MKD  },
    { "LIST", LIST },
    { "NLST", NLST },
    { "SITE", SITE },

     /* list argument */
    { "ALLO", ALLO }, /* list integers argument */
    { "STAT", STAT }, 
    { "HELP", HELP }, 
  };


  struct Utils {
    static type get_type(name t_command) {

      if(t_command >= ALLO)
        return list_argument;

      else if(t_command >= USER)
        return single_argument;

      else
        return no_argument;

    }

  };


}


#endif
