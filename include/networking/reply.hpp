#ifndef REPLY_HPP
#define REPLY_HPP

#include <map>
#include <string>

namespace networking {
namespace reply {
enum code {
  nocode,
  /* Init Request */
  r_100,
  r_110,
  r_120,
  r_125,
  r_150,

  /* Success */
  r_200,
  r_202,
  r_211,
  r_212,
  r_213,
  r_214,
  r_215,
  r_220,
  r_221,
  r_225,
  r_226,
  r_227,
  r_228,
  r_229,
  r_230,
  r_231,
  r_232,
  r_250,
  r_257,
  // removed: r_234

  /* Accepted request but on hold */
  r_300,
  r_331,
  r_332,
  r_350,

  /*  Not accepted and no action's taken place */
  r_400,
  r_421,
  r_425,
  r_426,
  r_430,
  r_434,
  r_450,
  r_451,
  r_452,
  r_500,

  /* Syntax Error / Command not recognized */
  r_501,
  r_502,
  r_503,
  r_504,
  r_530,
  r_532,
  r_550,
  r_552,
  r_553,
  // removed: r_534, r_551

  /* Reply regarding confidentiality and integrity */
  r_600,
  r_631,
  r_632,
  r_633,
};
static std::map<code, std::string> messages{

    /***** 100 series  ****/
    {r_100, "100 The requested action being initiated.\n"},
    {r_110, "110 Restart marker replay.\n"},
    {r_120, "120 Service ready.\n"},
    {r_125, "125 Data connection already open; transfer starting.\n"},
    {r_150, "150 File status okay; about to open data connection.\n"},

    /***** 200 series  ****/
    {r_200, "200 Ok\n"},
    {r_202, "202 Command not implemented, superfluous at this site.\n"},
    {r_211, "211"}, // append system features 
    {r_212, "212 Directory status: "},
    {r_213, "213 File status.\n"},
    {r_214, "214 "}, /* Append help message */
    {r_215, "215 "},     /* Append a system name*/
    {r_220, "220 My FTP Server. Ready for new user.\n"},
    {r_221, "221 Service closing control connection.\n"},
    {r_225, "225 Data connection open; no transfer in progress.\n"},
    {r_226, "226 Closing data connection. Requested file action successful.\n"},
    {r_227, "227 Entering Passive Mode.\n"},
    {r_228, "228 Entering Long Passive Mode.\n"},
    {r_229, "229 Entering Extended Passive Mode.\n"},
    {r_230, "230 User logged in, proceed.\n"},
    {r_231, "231 User logged out; service terminated.\n"},
    {r_232, "232 Logout command noted, will complete when transfer done.\n"},
    //{ r_234,	"Specifies that the server accepts the authentication mechanism
    //specifie"},
    {r_250, "250 Requested file action okay, completed.\n"},
    {r_257, "257 "}, /* append PATHNAME */

    /***** 300 series  ****/
    {r_300, "300 The command has been accepted, but the requested action is on " "hold, pending receipt of further information.\n"},
    {r_331, "331 User name okay, need password.\n"},
    {r_332, "332 Need account for login.\n"},
    {r_350, "350 Requested file action pending further information.\n"},

    /***** 400 series  ****/
    {r_400, "400 The command was not accepted. May be requested again.\n"},
    {r_421, "421 Service not available, closing control connection.\n"}, 
    {r_425, "425 Can't open data connection.\n"}, 
    {r_426, "426 Connection closed; transfer aborted."}, /* append details. */
    {r_430, "430 Invalid username or password.\n"},
    {r_434, "434 Requested host unavailable.\n"},
    {r_450, "450 Requested file action not taken.\n"},
    {r_451, "451 Requested action aborted. Local error in processing.\n"},
    {r_452, "452 Requested action not taken. Insufficient storage space.\n"},

    /***** 500 series  ****/
    {r_500, "500 Syntax error.\n"},
    {r_501, "501 Syntax error in parameters or arguments.\n"},
    {r_502, "502 Command not implemented.\n"},
    {r_503, "503 Bad sequence of commands.\n"},
    {r_504, "504 Command not implemented for that parameter.\n"},
    {r_530, "530 Not logged in.\n"},
    {r_532, "532 "},  // repurposed 532. See readme.
    //{ r_534,	"Could Not Connect to Server - Requires SSL."},
    {r_550, "550 Requested action not taken. File or directory does not exist.\n"},
    // { r_551,	"Requested action aborted. Page type unknown."},
    {r_552, "552 Requested file action aborted. Exceeded storage allocation.\n"},
    {r_553, "553 Requested action not taken. File name not allowed.\n"},

    /***** 600 series  ****/
    {r_600, "600 Replies regarding confidentiality and integrity.\n"},
    {r_631, "631 Integrity protected reply.\n"},
    {r_632, "632 Confidentiality and integrity protected reply.\n"},
    {r_633, "633 Confidentiality protected reply.\n"},
};

    struct Utils {
      static std::string append_message(reply::code reply_code, std::string &to_append) {
        std::string msg = messages.at(reply_code);

        return msg + to_append + "\n";
      }
    };

  } // namespace reply
} // namespace networking

#endif
