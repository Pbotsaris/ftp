#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main (void)
{
 // int socked_fd, nsocked_fd, port_num;
  struct sockaddr_in server_addr, client_addr;

  /* domain, type, protocol */
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM from TCP connection */
  int port      = 4000;

  if(socket_fd < 0)
      return 1;
  
  /* clear address struct */
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family      = AF_INET; /* AF_INET is the IPV4 family  */
  server_addr.sin_addr.s_addr = INADDR_ANY; /* the actual IP in the socket. Sets IP to localhost */
  server_addr.sin_port        = htons(port); /* conver port number to network byte order */


  if(bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
      return 1;


  listen(socket_fd, 5); /* maximum size of the backlog queue is 5 */

  socklen_t client_addr_len = sizeof(client_addr);

  int new_socket_fd = accept(socket_fd, (struct sockaddr*)&client_addr,  &client_addr_len);

   
  char buffer[256] = {0};
  int n = recv(new_socket_fd, buffer, sizeof(buffer)/sizeof(char), 0);

  if(n < 0)
      return 1;
   
  std::cout << buffer << "\n";

  char msg[] = "Response from server";
  send(new_socket_fd,msg, strlen(msg), 0);

  close(new_socket_fd);
  close(socket_fd);
  return 0;
}
