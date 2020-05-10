#include "NetCommon.h"

void nerror(const char *s)
{
  perror(s);
  exit(1);
}

NetCommon::NetCommon()
{
  sock_len = sizeof(remote_sock);

  if((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    nerror("socket");

  port = PORT;
}

NetCommon::~NetCommon()
{
  close(sock_fd);
}

int NetCommon::send(char *buffer)
{
  int retval = sendto(sock_fd, buffer, BUFLEN, 0, (sockaddr *)&remote_sock, sock_len);
  if(retval == -1) {
    nerror("sendto()");
  }
  return retval;
}

int NetCommon::recv(char *buffer)
{
  int retval = recvfrom(sock_fd, buffer, BUFLEN, 0, (sockaddr *)&remote_sock, &sock_len);
  if(retval == -1) {
    printf("recv(): packet from %s:%d\nData: %s\n\n",
           inet_ntoa(remote_sock.sin_addr), ntohs(remote_sock.sin_port), buffer);
  }
  return retval;
}

