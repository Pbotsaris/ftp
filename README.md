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
|STRU    	|	Set file transfer structure.                                                                                                                                                 |
|SYST    	|	Return system type.                                                                                                                                                          |
|THMB     | 	Streamlined FTP Command Extensions	Get a thumbnail of a remote image file                                                                                                 |
|TYPE    	|	Sets the transfer mode (ASCII/Binary).                                                                                                                                       |
|USER    	|	Authentication username.

