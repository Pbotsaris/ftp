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
//   ACCT = 12,    no support
     CWD  = 13,    // cd
//   REIN = 15,    no support
//   SMNT = 14,    no support
     PORT = 16,
     TYPE = 17,
//   STRU = 18,    no support, only single structure.
//   MODE = 19,    no support, stream only.
     RETR = 20,    // get
//   STOR = 21,    no support. Only unique files are created.
//   APPE = 23,    handled by client.
     REST = 24,    // restart
     STOU = 22,    // send
     RNFR = 25,    // rename
     RNTO = 26,    // rename
     DELE = 27,
     RMD  = 28,
     MKD  = 29,
     LIST = 30,
     NLST = 31,
//   SITE = 32,
//   ALLO = 33,  no support
     HELP = 34,  // rhelp (in ftp)
     STAT = 35,  // rstatus (in ftp)
  };

  enum type {
    no_argument,
    with_argument,
    zero_or_more_arguments,
  };

  static std::map<std::string, name > find {

    /* No arguments */
    { "CDUP", CDUP },  // done
    { "QUIT", QUIT },  // done
    { "PASV", PASV },
//  { "ABOR", ABOR },  no support
    { "PWD" , PWD  },  // done
    { "SYST", SYST },  // done
    { "NOOP", NOOP },
                      
    /* With argument */
    { "USER", USER },  // done
    { "PASS", PASS },  // done
//  { "ACCT", ACCT },  no support
    { "CWD", CWD },    // done
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
    { "RNFR", RNFR },  // done
    { "RNTO", RNTO },  // done
    { "DELE", DELE },
    { "RMD" , RMD  },  // done
    { "MKD" , MKD  },  // done
    { "LIST", LIST },
    { "NLST", NLST },
 // { "SITE", SITE },  no support
//  { "ALLO", ALLO },  no support
    { "STAT", STAT }, 
    { "HELP", HELP },  // done
  };

  static std::map<std::string, std::string > help {
    {  "CDUP", "Change to parent directory."}, 
    {  "QUIT", "Disconnects from server."}, 
    {  "PASV", "Enter passive mode."},
    {  "PWD", "Print working directory." }, 
    {  "SYST", "Get server operating system"}, 
    {  "NOOP", "No Operation (used mostly on keepalives)."},
    {  "USER", "Authenticate username."},
    {  "PASS", "Authenticate passowrd."},
    {  "CWD", "Change directory." },
    {  "PORT", "Specifies an address and port to which the server should connect."},
    {  "TYPE", "Sets the transfer mode to ASCII or Binary(Image)"},
    {  "RETR", "Retrives a copy of a file."},
    {  "STOU", "Stores a file uniquely."},
    {  "REST", "Restart the transfer for a specific point(marker)."},
    {  "RNFR", "Select the file to rename."},
    {  "RNTO", "Sets the new filename to rename to."},
    {  "DELE", "Deletes a file."},
    {  "RMD", "Removes a directory." },
    {  "MKD", "Creates a directory." },
    {  "LIST", "Returns info of a file or directory if specified, else info of the current working directory."},
    {  "NLST", "Returns a list of filenames in a specified directory."},
    {  "STAT", "Returns the server status, including the status of the current connection."}, 
    {  "HELP", "Display help of an specific command."}, 
  };

  struct Utils {


    static type get_type(name t_command) {

      if(t_command >= USER && t_command < LIST)
        return with_argument;

      else if(t_command >= LIST)
        return zero_or_more_arguments;

      else
        return no_argument;
    }
  };

}


#endif
