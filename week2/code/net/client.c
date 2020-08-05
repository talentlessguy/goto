#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

int process_request()
{

  int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket_fd == -1)
  {
    return 1;
  }

  struct addrinfo hints = {
      .ai_flags = 0,
      .ai_family = AF_INET,
      .ai_socktype = 0,
      .ai_protocol = IPPROTO_TCP

  };
  struct addrinfo *addr_info;

  if (getaddrinfo("127.0.0.1", "8080", &hints, &addr_info))
  {
    return 1;
  }

  struct sockaddr_in *server_info = (struct sockaddr_in *)addr_info->ai_addr;

  struct sockaddr_in servaddr = {
      .sin_family = AF_INET,
      .sin_addr = server_info->sin_addr,
      .sin_port = server_info->sin_port,
  };

  freeaddrinfo(addr_info);

  int resp;

  connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  read(socket_fd, &resp, sizeof(int));

  printf("received %d\n", resp);

  return 0;
}

int main()
{

  for (int i = 0; i < 100; i++)
  {
    pid_t pid = fork();

    process_request(pid);
  }

  return 0;
}