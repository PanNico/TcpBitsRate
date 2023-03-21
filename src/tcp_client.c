#include "tcp_client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static struct sockaddr_in g_servaddr;

int tcp_client_init(const char* srv_addr, int srv_port) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    return -1;
  }

  bzero(&g_servaddr, sizeof(g_servaddr));

  // assign IP, PORT
  g_servaddr.sin_family = AF_INET;
  g_servaddr.sin_addr.s_addr = inet_addr(srv_addr);
  g_servaddr.sin_port = htons(srv_port);

  return sockfd;
}

int tcp_client_connect(int sockfd) {
  if (connect(sockfd, (struct sockaddr*)&g_servaddr, sizeof(g_servaddr)) != 0) {
    return -1;
  }

  return 0;
}

void tcp_client_cleanup(int sockfd) { close(sockfd); }

int tcp_client_run(const char* srv_addr, int srv_port, void (*cb)(int)) {
  int sockfd = tcp_client_init(srv_addr, srv_port);

  if (sockfd < 0) return -1;

  if (tcp_client_connect(sockfd)) {
    return -2;
  }

  (*cb)(sockfd);
  tcp_client_cleanup(sockfd);

  return 0;
}

