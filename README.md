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


