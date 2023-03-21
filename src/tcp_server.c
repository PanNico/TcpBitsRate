#include "tcp_server.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int tcp_server_init(int port) {
  struct sockaddr_in servaddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (!sockfd) return -1;

  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
    return -2;
  }

  return sockfd;
}

void tcp_server_cleanup(int sockfd) { close(sockfd); }

int tcp_server_listen(int sockfd) {
  struct sockaddr_in cli;
  unsigned int len = sizeof(cli);

  if (listen(sockfd, 5) != 0) {
    return -1;
  }

  return accept(sockfd, (struct sockaddr*)&cli, &len);
}

int tcp_server_run(int port, void (*cb)(int), int times) {
  int sockfd = tcp_server_init(port);

  if (sockfd < 0) {
    printf("Failed to instantiate socket\n");
    return -3;
  }

  for (int i = 0; i < times; i++) {
    int connfd = tcp_server_listen(sockfd);

    if (connfd > 0) {
      (*cb)(connfd);
    }
  }

  tcp_server_cleanup(sockfd);
  return 0;
}

