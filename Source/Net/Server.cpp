#include "Server.h"

Server::Server() : NetCommon()
{
  memset((char *) &local_sock, 0, sizeof(local_sock));
  local_sock.sin_family = AF_INET;
  local_sock.sin_port = htons(PORT);
  local_sock.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock_fd, (sockaddr *)&local_sock, sizeof(local_sock))==-1)
    nerror("bind");
}


