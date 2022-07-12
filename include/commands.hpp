#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <string>

namespace commands {
  
  enum name {
    /* No arguments */
     QUIT = 0,     
     CDUP = 1,  
     PASV = 2,     
     PWD  = 3,
     SYST = 4,     
     NOOP = 5,

    /* With argument */
     USER = 6,
     PASS = 7,
     CWD  = 8,   
     PORT = 9,
     TYPE = 10,
     RETR = 11,    
     STOR = 12,    
     REST = 13,   
     STOU = 13,  
     RNFR = 14,    
     RNTO = 15,    
     DELE = 16,
     RMD  = 17,  
     MKD  = 18,

     /* zero or more arguments */
     LIST = 19,
     NLST = 21,
     HELP = 22,  
     STAT = 23,  
     FEAT = 24,  
                
     /* No Support */
     ABOR = 30,  
     ACCT = 31,
     REIN = 32,
     APPE = 33, 
     SMNT = 34, 
     STRU = 35, 
     MODE = 36, 
     SITE = 37, 
     ALLO = 38, 
     LPRT = 39,
  };

  enum type {
    no_argument,
    with_argument,
    zero_or_more_arguments,
    no_support,
  };

  static std::map<std::string, name > find {

    /* No arguments */
    { "CDUP", CDUP },  
    { "QUIT", QUIT },  
    { "PASV", PASV },
    { "PWD" , PWD  },  
    { "SYST", SYST },  
    { "NOOP", NOOP },
                      
    /* With argument */
    { "USER", USER },  
    { "PASS", PASS },  
    { "CWD", CWD },    
    { "PORT", PORT },  
    { "TYPE", TYPE },
    { "RETR", RETR }, 
    { "STOU", STOU },
    { "STOR", STOR },  
    { "REST", REST },
    { "RNFR", RNFR },  
    { "RNTO", RNTO },  
    { "DELE", DELE },
    { "RMD" , RMD  },  
    { "MKD" , MKD  },  

    /* zero or more arguments */
    { "LIST", LIST },
    { "NLST", NLST },
    { "STAT", STAT }, 
    { "HELP", HELP },  
    { "FEAT", FEAT },  
                       
    /* No Support */
    { "ABOR", ABOR }, 
    { "ACCT", ACCT }, 
    { "SMNT", SMNT }, 
    { "STRU", STRU }, 
    { "MODE", MODE }, 
    { "REIN", REIN }, 
    { "APPE", APPE }, 
    { "SITE", SITE }, 
    { "ALLO", ALLO }, 
    { "LPRT", LPRT }, 
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
    {  "FEAT", "Responds with server features."}, 

    // NO SUPPORT 
    { "ABOR", "No support." }, 
    { "ACCT", "No support." }, 
    { "SMNT", "No support." }, 
    { "STRU", "No support." }, 
    { "MODE", "No support." }, 
    { "REIN", "No support." }, 
    { "APPE", "No support." }, 
    { "SITE", "No support." }, 
    { "ALLO", "No support." }, 
    { "LPRT", "No support." }, 
  };

  struct Utils {
    static type get_type(name t_command) {

      if(t_command >= USER && t_command < LIST)
        return with_argument;

      else if(t_command >= LIST && t_command < ABOR)
        return zero_or_more_arguments;

      else if(t_command >= ABOR)
        return no_support;

      else
        return no_argument;
    }
  };

}

#endif
