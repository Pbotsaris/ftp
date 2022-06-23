#ifndef REPLY_HPP
#define REPLY_HPP

#include <map>
#include <string>

namespace reply {
  enum code {
    nocode,
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
    static std::map<code, std::string> messages {

    /***** 100 series  ****/
    { r_100,  "The requested action being initiated.\n"},                                                          
    { r_110,	"Restart marker replay.\n"},                                                       
    { r_120,	"Service ready in nnn minutes.\n"},                                                                                                                                
    { r_125,	"Data connection already open; transfer starting.\n"},                                                                                                             
    { r_150,	"File status okay; about to open data connection.\n"},

    /***** 200 series  ****/
    { r_200,  "Ok\n"},                                                                                                      
    { r_202,	"Command not implemented, superfluous at this site.\n"},                                                                                                           
    { r_211,	"System status, or system help reply.\n"},                                                              
    { r_212,	"Directory status.\n"},                                                                                                                                            
    { r_213,	"File status.\n"},                                
    { r_214,	"Help message:  \n"},  /* Append help message */             
    { r_215,	" system type\n"},   /* Prepend a system type NAME*/                                                                    
    { r_220,	"Service ready for new user.\n"},                                                                                                                                  
    { r_221,	"Service closing control connection.\n"},                                                                                                                          
    { r_225,	"Data connection open; no transfer in progress.\n"},                                                                                                               
    { r_226,	"Closing data connection. Requested file action successful.\n"},
    { r_227,	"Entering Passive Mode.\n"},                                                                                                                   
    { r_228,	"Entering Long Passive Mode.\n"},                                                                                                            
    { r_229,	"Entering Extended Passive Mode.\n"},                                                                                                                   
    { r_230,	"User logged in, proceed.\n"},                                                                                                          
    { r_231,	"User logged out; service terminated.\n"},                                                                                                                         
    { r_232,	"Logout command noted, will complete when transfer done.\n"},                                                                                                      
    //{ r_234,	"Specifies that the server accepts the authentication mechanism specifie"},                       
    { r_250,	"Requested file action okay, completed.\n"},                                                                                                                       
    { r_257,	"  created.\n"}, /* Prepend PATHNAME */                                                                                                                                       

    /***** 300 series  ****/
    { r_300,  "The command has been accepted, but the requested action is on hold, pending receipt of further information.\n"},                                                 
    { r_331,	"User name okay, need password.\n"},                                                                                                                               
    { r_332,	"Need account for login.\n"},                                                                                                                                      
    { r_350,	"Requested file action pending further information.\n"},                                                                                                            

    /***** 400 series  ****/
    { r_400,  "The command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again.\n"},        
    { r_421,	"Service not available, closing control connection.\n"}, /* This may be a reply to any command if the service knows it must shut down. */                                
    { r_425,	"Can't open data connection.\n"},                                                                                                                                  
    { r_426,	"Connection closed; transfer aborted.\n"},                                                                                                                         
    { r_430,	"Invalid username or password.\n"},                                                                                                                                 
    { r_434,	"Requested host unavailable.\n"},                                                                                                                                  
    { r_450,	"Requested file action not taken.\n"},                                                                                                                             
    { r_451,	"Requested action aborted. Local error in processing.\n"},                                                                                                         
    { r_452,	"Requested action not taken. Insufficient storage space in system. File unavailable (e.g., file busy).\n"},                                                        

    /***** 500 series  ****/
    { r_500,  "Syntax error.\n"},
    { r_501,	"Syntax error in parameters or arguments.\n"},                                                                                                                     
    { r_502,	"Command not implemented.\n"},                                                                                                                                     
    { r_503,	"Bad sequence of commands.\n"},                                                                                                                                    
    { r_504,	"Command not implemented for that parameter.\n"},                                                                                                                  
    { r_530,	"Not logged in.\n"},                                                                                                                                               
    { r_532,	"Need account for storing files.\n"},                                                                                                                              
    //{ r_534,	"Could Not Connect to Server - Requires SSL."},                                                                                                            
    { r_550,	"Requested action not taken. File unavailable (e.g., file not found, no access).\n"},                                                                              
    // { r_551,	"Requested action aborted. Page type unknown."},                                                                                                              
    { r_552,	"Requested file action aborted. Exceeded storage allocation (for current directory or dataset).\n"}, 
    { r_553,	"Requested action not taken. File name not allowed.\n"},                                                                                                           

    /***** 600 series  ****/
    { r_600,   "Replies regarding confidentiality and integrity.\n"},
    { r_631,	"Integrity protected reply.\n"},                                                                                                                                   
    { r_632,	"Confidentiality and integrity protected reply.\n"},                                                                                                               
    { r_633,	"Confidentiality protected reply.\n"},                                                                                                                             
  };
}

#endif
