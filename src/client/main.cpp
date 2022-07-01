#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 4000;
const char LOCALHOST[] = "127.0.0.1";

int main(void)
{
  struct sockaddr_in client_addr;

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);

  /* clear address struct */
  bzero(&client_addr, sizeof(client_addr));

  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT);

  inet_pton(AF_INET, LOCALHOST, &client_addr.sin_addr); /* set address to localhost */

  int client_fd = connect(client_socket, reinterpret_cast<struct sockaddr *>(&client_addr), sizeof(client_addr));

  if(client_fd < 0)
    std::cout << "connection error" << "\n";

  char msg[] = "Hello from client! very long how much time I can do this because is longer than the buffer I have and will it create a problem\r\n";
  char buffer[256] = {0};

   send(client_socket, msg, strlen(msg), 0);

   recv(client_socket, buffer, 256, 0);

  std::cout << buffer << "\n";

  close(client_fd);
  close(client_socket);

  return 0;
}
