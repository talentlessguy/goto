#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <sys/types.h>

int main()
{

  // setup start
  int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct addrinfo hints = {
      .ai_flags = 0,
      .ai_family = AF_INET,
      .ai_socktype = 0,
      .ai_protocol = IPPROTO_TCP,
  };

  struct addrinfo *addr_info;
  if (getaddrinfo("127.0.0.1", "8080", &hints, &addr_info))
  {
    return 1;
  }

  struct sockaddr_in *server_info =
      (struct sockaddr_in *)addr_info->ai_addr;

  struct sockaddr_in server_addr = {
      .sin_family = AF_INET,
      .sin_addr = server_info->sin_addr,
      .sin_port = server_info->sin_port};

  freeaddrinfo(addr_info);

  bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  listen(socket_fd, 10);

  // setup end

  while (1)
  {
    struct sockaddr_in client;
    unsigned int addr_len = sizeof(client);
    int client_fd = accept(socket_fd, (struct sockaddr *)&client, &addr_len);

    int result = 10;

    write(client_fd, &result, sizeof(int));

    close(client_fd);
  }

  close(socket_fd);
  return 0;
}