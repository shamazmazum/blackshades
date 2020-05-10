#include "Client.h"

#define SRV_IP "0.0.0.0"

Client::Client()
{
  NetCommon::NetCommon();

  memset((char *)&remote_sock, 0, sizeof(remote_sock));
  remote_sock.sin_family = AF_INET;
  remote_sock.sin_port = htons(port);
  if (inet_aton(SRV_IP, &remote_sock.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
}

