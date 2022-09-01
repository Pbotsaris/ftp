# My FTP

An multithreaded FTP server following the [RFC959](https://www.w3.org/Protocols/rfc959/) protocol. 

## Authentication

The server require users to authenticate. Below the list of default users; anonymouys does not require a password.

| user      | password  |
|-----------|-----------|
| anonymous |           |
| pedro     |  drops    |
| gaetan    |  qwasar   |

New users may be added in the `.users` config file. Each user entry has a `user` and `password` field delimited by a `:`. Users are separated by a new line(`\n`) like so

      user1:password1
      user2:passowrd2

To create users without password set the password to `0`

      user_without_password:0

## The root directory 

My FTP creates a directory `.root/` in the same directory as the running binary. Clients only have access to files within `.root/` while
the remaining of the server's filesystem remain private and inaccessible. This is reflected in perspective of what the absolute path is between the client and server.

Server absolute path

      /home/username/.root/file.jpg

Client absolute path
  
     file.jpg

Clients are unable to `CDUP` out of the `.root` directory.

## Logging

The server has 3 types of log messages: `Error`(red),`Info`(blue) and `Debug`(yellow). The `Debug` logs are somewhat noisy and they can be turned on or
off by removing/adding the macro `#define DEBUG` to/from the `include/logger.hpp` header.

## Build & run

Build

      make

Run

     ./bin/ftp <port-number>

Run server with unit testing
 
    ./bin/ftp <port-number> --test

Clean

     make clean


## Implementation details

The server listens to new connections (`accept`) as well as to connected sockets control requests in the main thread. This logic is ran in a single thread using a polling system. 
Once a request is received from a client, the server will create a `Service` object and dispatch it to a threadpool. And available thread will then call `Service.work()` to execute this request. 

Note that only the control connection is managed in the main thread. Data connection are always established from a separated thread within the context of a `Service`.

The program revolves around modifying a `Request` object that is created when the server first receives a message from
`Connection`. This `Request` is then populated with the request info such as the socket fd of the requesting client, data received etc
and is passed on to a `Service` that, in its turn will parse and route this request to specific controller.

## Thread Throttling 

The threadpool will use all the available threads in you system with `std::thread::hardware_concurrency`. To avoid the CPU usage going out of control, the threadpool implements a simple throttling logic.
As the server goes idle the loop will checking for new jobs will get throttle with longer and longer sleep times from 50ms all the way to 500ms.

## Accepted Commands

| Command | Description                                                                                                                                                                  |
|---------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
 | CDUP   | Change to parent directory                                                                                                                                                   |
 | QUIT   | Disconnects from server                                                                                                                                                      |
 | PASV   | Enter passive mode                                                                                                                                                           |
 | PWD    | Print working directory                                                                                                                                                      |
 | SYST   | Get server operating system                                                                                                                                                  |
 | NOOP   | No Operation (used mostly on keepalives)                                                                                                                                     |
 | USER   | Authenticate username                                                                                                                                                        |
 | PASS   | Authenticate passowrd                                                                                                                                                        |
 | CWD    | Change directory                                                                                                                                                             |
 | PORT   | Specifies an address and port to which the server should connect                                                                                                             |
 | TYPE   | Sets the transfer mode to ASCII or Binary(Image)                                                                                                                             |
 | RETR   | Retrives a copy of a file                                                                                                                                                    |
 | STOU   | Stores a file uniquely                                                                                                                                                       |
 | RNFR   | Select the file to rename                                                                                                                                                    |
 | RNTO   | Sets the new filename to rename to                                                                                                                                           |
 | DELE   | Deletes a file                                                                                                                                                               |
 | RMD    | Removes a directory                                                                                                                                                          |
 | MKD    | Creates a directory                                                                                                                                                          |
 | LIST   | Returns info of a file or directory if specified, else info of the current working directory                                                                                 |
 | NLST   | Returns a list of filenames in a specified directory                                                                                                                         |
 | STAT   | Returns the server status, including the status of the current connection                                                                                                    |
 | HELP   | Display help of an specific command                                                                                                                                          |
 | FEAT   | Responds with server features                                                                                                                                                |
 | ABOR   | No support                                                                                                                                                                   |
 | ACCT   | No support                                                                                                                                                                   |
 | SMNT   | No support                                                                                                                                                                   |
 | STRU   | No support                                                                                                                                                                   |
 | MODE   | No support                                                                                                                                                                   |
 | REIN   | No support                                                                                                                                                                   |
 | APPE   | No support                                                                                                                                                                   |
 | SITE   | No support                                                                                                                                                                   |
 | ALLO   | No support                                                                                                                                                                   |
 | LPRT   | No support                                                                                                                                                                   |
 | LPSV   | No support                                                                                                                                                                   |
 | REST   | No support                                                                                                                                                                   |
                                                                                                                                                                                        
### Replies

|  Reply    | Description
|-----------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
100 series  | The requested action is being initiated, expect another reply before proceeding with a new command.                                                                           |
110         |	Restart marker replay . In this case, the text is exact and not left to the particular implementation;                                                                        |
120         |	Service ready in nnn minutes.                                                                                                                                                 |
125         |	Data connection already open; transfer starting.                                                                                                                              |
150         |	File status okay; about to open data connection.                                                                                                                              |
200 series  |	The requested action has been successfully completed.                                                                                                                       |
202         |	Command not implemented, superfluous at this site.                                                                                                                            |
211         |	System status, or system help reply.                                                                                                                                          |
212         |	Directory status.                                                                                                                                                             |
213         |	File status.                                                                                                                                                                  |
214         |	Help message. Explains how to use the server or the meaning of a particular non-standard command. This reply is useful only to the human user.                                |
215         |	NAME system type. Where NAME is an official system name from the registry kept by IANA.                                                                                       |
220         |	Service ready for new user.                                                                                                                                                   |
221         |	Service closing control connection.                                                                                                                                           |
225         |	Data connection open; no transfer in progress.                                                                                                                                |
226         |	Closing data connection. Requested file action successful (for example, file transfer or file abort).                                                                         |
227         |	Entering Passive Mode (h1,h2,h3,h4,p1,p2).                                                                                                                                    |
228         |	Entering Long Passive Mode (long address, port).                                                                                                                              |
229         |	Entering Extended Passive Mode (|||port|).                                                                                                                                    |
230         |	User logged in, proceed. Logged out if appropriate.                                                                                                                           |
231         |	User logged out; service terminated.                                                                                                                                          |
232         |	Logout command noted, will complete when transfer done.                                                                                                                       |
234         |	Specifies that the server accepts the authentication mechanism specified by the client, and the exchange of security data is complete.                                        |
250         |	Requested file action okay, completed.                                                                                                                                        |
257         |	"PATHNAME" created.                                                                                                                                                           |
300 series  |  The command has been accepted, but the requested action is on hold, pending receipt of further information.                                                                  |
331         |	User name okay, need password.                                                                                                                                                |
332         |	Need account for login.                                                                                                                                                       |
350         |	Requested file action pending further information                                                                                                                             |
400 series  | The command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again.                 |
421         |	Service not available, closing control connection. This may be a reply to any command if the service knows it must shut down.                                                 |
425         |	Can't open data connection.                                                                                                                                                   |
426         |	Connection closed; transfer aborted.                                                                                                                                          |
430         |	Invalid username or password                                                                                                                                                  |
434         |	Requested host unavailable.                                                                                                                                                   |
450         |	Requested file action not taken.                                                                                                                                              |
451         |	Requested action aborted. Local error in processing.                                                                                                                          |
452         |	Requested action not taken. Insufficient storage space in system. File unavailable (e.g., file busy).                                                                         |
500 series  | Syntax error, command unrecognized and the requested action did not take place. This may include errors such as command line too long.                                        |
501         |	Syntax error in parameters or arguments.                                                                                                                                      |
502         |	Command not implemented.                                                                                                                                                      |
503         |	Bad sequence of commands.                                                                                                                                                     |
504         |	Command not implemented for that parameter.                                                                                                                                   |
530         |	Not logged in.                                                                                                                                                                |
532         |	Need account for storing files.                                                                                                                                               |
534         |	Could Not Connect to Server - Policy Requires SSL                                                                                                                             |
550         |	Requested action not taken. File unavailable (e.g., file not found, no access).                                                                                               |
551         |	Requested action aborted. Page type unknown.                                                                                                                                  |
552         |	Requested file action aborted. Exceeded storage allocation (for current directory or dataset).                                                                                |
553         |	Requested action not taken. File name not allowed.                                                                                                                            |
600 series  | Replies regarding confidentiality and integrity                                                                                                                       |
631         |	Integrity protected reply.                                                                                                                                                    |
632         |	Confidentiality and integrity protected reply.                                                                                                                                |
633         |	Confidentiality protected reply.                                                                                                                                              |


