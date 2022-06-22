#include "../../include/server.h"

int main (void)
{
  auto server = Server(4000, 5);

  server.set_socket_options();
  server.config_addr();
  server.bind_socket();
  server.slisten();
  server.receive();
}


// int socked_fd, nsocked_fd, port_num;
//  struct sockaddr_in server_addr, client_addr;
//
//  /* domain, type, protocol */
//  int socket_fd = socket(AF_INET, SOCK_STREAM, 0); /* SOCK_STREAM from TCP connection */
//  int res = 0;
//
//  if(socket_fd < 0)
//      return 1;
//  
//  /* clear address struct */
//  bzero(&server_addr, sizeof(server_addr));
//
//  int opt = 1;
//  res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//
//  if(res < 0)
//  {
//    perror("error setting options");
//    return 1;
//  }
//
//  res = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
//
//  if(res < 0)
//  {
//    perror("error setting options");
//    return 1;
//  }
//
//
//  if(bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
//    std::cout << "error binding" << "\n";
//
//
//
//  if(res < 0)
//    std::cout << "error listening" << "\n";
//
//
//  socklen_t client_addr_len = sizeof(client_addr);
//
//  int new_socket_fd = accept(socket_fd, (struct sockaddr*)&client_addr,  &client_addr_len);
//
//  if(new_socket_fd < 0)
//    std::cout << "error accepting" << "\n";
//
//  char buffer[256] = {0};
//  int n = recv(new_socket_fd, buffer, sizeof(buffer)/sizeof(char), 0);
//
//  if(n < 0)
//    return 1;
//
//  std::cout << buffer << "\n";
//
//  char msg[] = "Response from server";
//  send(new_socket_fd,msg, strlen(msg), 0);
//
//  close(new_socket_fd);
//  shutdown(socket_fd, SHUT_RDWR);
//  return 0;
//
