#ifndef REPLY_HPP
#define REPLY_HPP

#include <map>
#include <string>

namespace reply {
  enum code {
    /* Init Request */
    r_100, r_110, r_120, r_125, r_150,
  
    /* Success */
    r_200, r_202, r_211, r_212, r_213,
    r_214, r_215, r_220, r_221, r_225,
    r_226, r_227, r_228, r_229, r_230,
    r_231, r_232, r_250, r_257,
    // removed: r_234
  
    /* Accepted request but on hold */
    r_300, r_331, r_332, r_350,
  
    /*  Not accepted and no action's taken place */
    r_400, r_421, r_425, r_426, r_430,
    r_434, r_450, r_451, r_452, r_500,
  
    /* Syntax Error / Command not recognized */
    r_501, r_502, r_503, r_504, r_530,
    r_532, r_550, r_552, r_553, 
    // removed: r_534, r_551
    
    /* Reply regarding confidentiality and integrity */
    r_600, r_631, r_632, r_633,
  };

  
 const std::map<code, std::string> MESSAGES {

    /***** 100 series  ****/
    { r_100,  "The requested action being initiated."},                                                          
    { r_110,	"Restart marker replay."},                                                       
    { r_120,	"Service ready in nnn minutes."},                                                                                                                                
    { r_125,	"Data connection already open; transfer starting."},                                                                                                             
    { r_150,	"File status okay; about to open data connection."},
    { r_200,  ""},                                                                                                      

    /***** 200 series  ****/
    { r_202,	"Command not implemented, superfluous at this site."},                                                                                                           
    { r_211,	"System status, or system help reply."},                                                              
    { r_212,	"Directory status."},                                                                                                                                            
    { r_213,	"File status."},                                
    { r_214,	"Help message:  "},  /* Append help message */             
    { r_215,	" system type"},   /* Prepend a system type NAME*/                                                                    
    { r_220,	"Service ready for new user."},                                                                                                                                  
    { r_221,	"Service closing control connection."},                                                                                                                          
    { r_225,	"Data connection open; no transfer in progress."},                                                                                                               
    { r_226,	"Closing data connection. Requested file action successful."},
    { r_227,	"Entering Passive Mode."},                                                                                                                   
    { r_228,	"Entering Long Passive Mode."},                                                                                                            
    { r_229,	"Entering Extended Passive Mode."},                                                                                                                   
    { r_230,	"User logged in, proceed."},                                                                                                          
    { r_231,	"User logged out; service terminated."},                                                                                                                         
    { r_232,	"Logout command noted, will complete when transfer done."},                                                                                                      
    //{ r_234,	"Specifies that the server accepts the authentication mechanism specifie"},                       
    { r_250,	"Requested file action okay, completed."},                                                                                                                       
    { r_257,	"  created."}, /* Prepend PATHNAME */                                                                                                                                       

    /***** 300 series  ****/
    { r_300,  "The command has been accepted, but the requested action is on hold, pending receipt of further information."},                                                 
    { r_331,	"User name okay, need password."},                                                                                                                               
    { r_332,	"Need account for login."},                                                                                                                                      
    { r_350,	"Requested file action pending further information."},                                                                                                            

    /***** 400 series  ****/
    { r_400,  "The command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again."},        
    { r_421,	"Service not available, closing control connection."}, /* This may be a reply to any command if the service knows it must shut down. */                                
    { r_425,	"Can't open data connection."},                                                                                                                                  
    { r_426,	"Connection closed; transfer aborted."},                                                                                                                         
    { r_430,	"Invalid username or password."},                                                                                                                                 
    { r_434,	"Requested host unavailable."},                                                                                                                                  
    { r_450,	"Requested file action not taken."},                                                                                                                             
    { r_451,	"Requested action aborted. Local error in processing."},                                                                                                         
    { r_452,	"Requested action not taken. Insufficient storage space in system. File unavailable (e.g., file busy)."},                                                        

    /***** 500 series  ****/
    { r_500,  "Syntax error."},
    { r_501,	"Syntax error in parameters or arguments."},                                                                                                                     
    { r_502,	"Command not implemented."},                                                                                                                                     
    { r_503,	"Bad sequence of commands."},                                                                                                                                    
    { r_504,	"Command not implemented for that parameter."},                                                                                                                  
    { r_530,	"Not logged in."},                                                                                                                                               
    { r_532,	"Need account for storing files."},                                                                                                                              
    //{ r_534,	"Could Not Connect to Server - Requires SSL."},                                                                                                            
    { r_550,	"Requested action not taken. File unavailable (e.g., file not found, no access)."},                                                                              
    // { r_551,	"Requested action aborted. Page type unknown."},                                                                                                              
    { r_552,	"Requested file action aborted. Exceeded storage allocation (for current directory or dataset)."}, 
    { r_553,	"Requested action not taken. File name not allowed."},                                                                                                           

    /***** 600 series  ****/
    { r_600,   "Replies regarding confidentiality and i}ntegrity."},
    { r_631,	"Integrity protected reply."},                                                                                                                                   
    { r_632,	"Confidentiality and integrity protected reply."},                                                                                                               
    { r_633,	"Confidentiality protected reply."},                                                                                                                             
  };
}

#endif
