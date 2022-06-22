#include <cstdio>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 4000;


int main (void)
{
 // int socked_fd, nsocked_fd, port_num;
  struct sockaddr_in server_addr, client_addr;

  /* domain, type, protocol */
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM from TCP connection */
  int res = 0;

  if(socket_fd < 0)
      return 1;
  
  /* clear address struct */
  bzero(&server_addr, sizeof(server_addr));

  int opt = 1;
  res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(res < 0)
  {
    perror("error setting options");
    return 1;
  }

  res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  if(res < 0)
  {
    perror("error setting options");
    return 1;
  }

  server_addr.sin_family      = AF_INET; /* AF_INET is the IPV4 family  */
  server_addr.sin_addr.s_addr = INADDR_ANY; /* the actual IP in the socket. Sets IP to localhost */
  server_addr.sin_port        = htons(PORT); /* conver port number to network byte order */


  if(bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    std::cout << "error binding" << "\n";


  res = listen(socket_fd, 5); /* maximum size of the backlog queue is 5 */

  if(res < 0)
    std::cout << "error listening" << "\n";


  socklen_t client_addr_len = sizeof(client_addr);

  int new_socket_fd = accept(socket_fd, (struct sockaddr*)&client_addr,  &client_addr_len);

  if(new_socket_fd < 0)
    std::cout << "error accepting" << "\n";

  char buffer[256] = {0};
  int n = recv(new_socket_fd, buffer, sizeof(buffer)/sizeof(char), 0);

  if(n < 0)
    return 1;

  std::cout << buffer << "\n";

  char msg[] = "Response from server";
  send(new_socket_fd,msg, strlen(msg), 0);

  close(new_socket_fd);
  shutdown(socket_fd, SHUT_RDWR);
  return 0;
}
