#ifndef _NET_NETCOMMON_H_
#define _NET_NETCOMMON_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 512
#define PORT 9930

void nerror(const char *s);

class NetCommon
{
  public:
  NetCommon();
  ~NetCommon();

  int send(char *buffer);
  int recv(char *buffer);

  protected:
  int sock_fd;
  struct sockaddr_in local_sock;
  struct sockaddr_in remote_sock;
  socklen_t sock_len;
  unsigned int port;
};

#endif /*_NET_NETCOMMON_H_*/
