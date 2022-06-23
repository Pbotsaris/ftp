# My FTP

write a description of the app here.

### File structure

For now, I separated the file structure between `client` and `server`. The client side is only here for testing as we still have a barebones of a server
that CANNOT yet handle a proper FTP connection. 

After the server is properly implemented the client implementation will go away and we will use any FTP client out there to connect.

### Server class
For now we have a simple implementation that handles a single request. I already wrapped up the socket code in a class `src/server/server.cpp`.
the header can be found in `include/server.h`.

### Compiling and running current implementation

Compile

     make

The `Makefile` will produce 2 binaries – `bin/ftp`(server) and `bin/ftp_client`. First run the server

      ./bin/ftp

The server will hang waiting for a connection. Now, open another shell window and run the client

      ./bin/ftp_client

You should see the exchange of messages between the client and server in your terminal


      Hello from server!
      Received!

## Next Steps

1. Implement a loop for the server so it can handle a multple connections. The loop should run through the whole connection lifecycle - `listen`, `accept` ,`recv` and `send`
on every connection request.

2. Build a server that handles 2 connections. FTP server establish 2 connections (where HTTP only a single connection) on TCP ports `20` and `21`. One connection is for the data while the second is 
  the control connection.

3. Understand the FTP protocol so we can can build a server around that standard. 

**Below some literature**:

[The protocol](https://www.w3.org/Protocols/rfc959/)

[wikipedia](https://en.wikipedia.org/wiki/File_Transfer_Protocol)

[difference between FTP and HTTP](https://techdifferences.com/difference-between-http-and-ftp.html#:~:text=The%20basic%20difference%20between%20HTTP,as%20well%20as%20control%20connection.)

[FTP explained](https://www.techtarget.com/searchnetworking/definition/File-Transfer-Protocol-FTP)

[FTP tutorial](https://www.hostinger.com/tutorials/what-is-ftp)

[More info](https://www.ncftp.com/libncftp/doc/ftp_overview.html)

[Protocol](https://datatracker.ietf.org/doc/rfc959/)

## List with FTP accepted command

| Command | Description                                                                                                                                                                  |
|---------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|ABOR    	|	Abort an active file transfer.                                                                                                                                               |
|ACCT    	|	Account information.                                                                                                                                                         |
|ALLO    	|	Allocate sufficient disk space to receive a file.                                                                                                                            |
|APPE    	|	Append (with create)                                                                                                                                                         |
|AVBL     |	Streamlined FTP Command Extensions	Get the available space                                                                                                                  |
|CDUP    	|	Change to Parent Directory.                                                                                                                                                  |
|CSID     |	Streamlined FTP Command Extensions	Client / Server Identification                                                                                                           |
|DELE    	|	Delete file.                                                                                                                                                                 |
|DSIZ     |	Streamlined FTP Command Extensions	Get the directory size                                                                                                                   |
|HELP    	|	Returns usage documentation on a command if specified, else a general help document is returned.                                                                             |
|LIST    	|	Returns information of a file or directory if specified, else information of the current working directory is returned.                                                      |
|MFCT     |	The 'MFMT', 'MFCT', and 'MFF' Command Extensions for FTP	Modify the creation time of a file.                                                                                |
|MFF	     |  The 'MFMT', 'MFCT', and 'MFF' Command Extensions for FTP	Modify fact (the last modification time, creation time, UNIX group/owner/mode of a file).                        |
|MFMT     |  The 'MFMT', 'MFCT', and 'MFF' Command Extensions for FTP	Modify the last modification time of a file.                                                                       |
|MKD	     | 	Make directory.                                                                                                                                                            |
|MODE    	|	Sets the transfer mode (Stream, Block, or Compressed).                                                                                                                       |
|NLST    	|	Returns a list of file names in a specified directory.                                                                                                                       |
|NOOP    	|	No operation (dummy packet; used mostly on keepalives).                                                                                                                      |
|PASS    	|	Authentication password.                                                                                                                                                     |
|PASV    	|	Enter passive mode.                                                                                                                                                          |
|PORT    	|	Specifies an address and port to which the server should connect.                                                                                                            |
|PWD	     | 	Print working directory. Returns the current directory of the host.                                                                                                        |
|QUIT    	|	Disconnect.                                                                                                                                                                  |
|REIN    	|	Re initializes the connection.                                                                                                                                               |
|RETR    	|	Retrieve a copy of the file                                                                                                                                                  |
|RMD	     | 	Remove a directory.                                                                                                                                                        |
|RMDA     | 	Streamlined FTP Command Extensions	Remove a directory tree                                                                                                                |
|RNFR    	|	Rename from.                                                                                                                                                                 |
|RNTO    	|	Rename to.                                                                                                                                                                   |
|SITE    	|	Sends site specific commands to remote server (like SITE IDLE 60 or SITE UMASK 002). Inspect SITE HELP output for complete list of supported commands.                       |
|SMNT    	|	Mount file structure.                                                                                                                                                        |
|SPSV     | 	FTP Extension Allowing IP Forwarding (NATs)	Use single port passive mode (only one TCP port number for both control connections and passive-mode data connections)         |
|STAT    	|	Returns information on the server status, including the status of the current connection                                                                                     |
|STOR    	|	Accept the data and to store the data as a file at the server site                                                                                                           |
|STOU    	|	Store file uniquely.                                                                                                                                                         |
|~STRU~ 	|	~Set file transfer structure.~(DO NOT IMPLEMENT)                                                                                                                               |
|SYST    	|	Return system type.                                                                                                                                                          |
|THMB     | 	Streamlined FTP Command Extensions	Get a thumbnail of a remote image file                                                                                                 |
|TYPE    	|	Sets the transfer mode (ASCII/Binary).                                                                                                                                       |
|USER    	|	Authentication username.

### FTP Replies

|  Reply    | Description
|-----------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
100 series  | The requested action is being initiated, expect another reply before proceeding with a new command.                                                                           |
110         |	Restart marker replay . In this case, the text is exact and not left to the particular implementation;                                                                        |
120         |	Service ready in nnn minutes.                                                                                                                                                 |
125         |	Data connection already open; transfer starting.                                                                                                                              |
150         |	File status okay; about to open data connection.                                                                                                                              |
200 series  | 	The requested action has been successfully completed.                                                                                                                       |
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


## Datatypes
While transfering data, the FTP supports a few datatypes. Some of them are undated and will not be mentioned here. The revelevant ones are as follows

**ASCII (TYPE A)**: Used for text.  This type has optional formats that were intended for printers and not relevant to this implemtation. The program will 
use the **Non-print** only(aka`format control of N`).

**Image or Binary mode (TYPE I)** The sending machine sends each file byte by byte, and the recipient stores the bytestream as it receives it. This is our main file transfer mode.

## File transfer structure
Organizational types to be implemented is the `F` or **FILE** which is the file structure in a UNIX system. 

## Log in 
Uses the `USER` and `PASS` command for for authetication. May also provide an anonymous access.

## Data Transfers

**Stream mode (MODE S)**: Data is sent as a continuous stream, relieving FTP from doing any processing. Rather, all processing is left up to TCP. No End-of-file indicator is needed, unless the data is divided into records.

**Block mode (MODE B)**: Designed primarily for transferring record-oriented files (STRU R), although can also be used to transfer stream-oriented (STRU F) text files. FTP puts each record (or line) of data into several blocks (block header, byte count, and data field) and then passes it on to TCP.[8]

**Compressed mode (MODE C)**: Extends MODE B with data compression using run-length encoding.

Most contemporary FTP clients and servers do not implement MODE B or MODE C.

### Stream mode

         In a record structured file EOR and EOF will each be indicated
         by a two-byte control code.  The first byte of the control code
         will be all ones, the escape character.  The second byte will
         have the low order bit on and zeros elsewhere for EOR and the
         second low order bit on for EOF; that is, the byte will have
         value 1 for EOR and value 2 for EOF.  EOR and EOF may be
         indicated together on the last byte transmitted by turning both
         low order bits on (i.e., the value 3).  If a byte of all ones
         was intended to be sent as data, it should be repeated in the
         second byte of the control code.


## Syntax

`ftp://[user[:password]@]host[:port]/url-path`

