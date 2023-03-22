#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_actors.h"
#include "tcp_server.h"
#include "traffic_gen.h"

static char buf[SELECTED_FLOW_SZ] = {0};

void print_help() { printf("./calc_rate_srv <port>\n"); }

int server_auth(int sockfd) {
  int bytes_read = 0;
  uint8_t ret = 1;
  while ((bytes_read += read(sockfd, buf + bytes_read,
                             SELECTED_FLOW_SZ - bytes_read)) < SELECTED_FLOW_SZ)
    ;

  if (strcmp(buf, "download") == 0) {
    traffic_gen_init(1);
    ret = 0;
  } else if (strcmp(buf, "upload") == 0) {
    traffic_gen_init(0);
    ret = 0;
  }

  write(sockfd, &ret, 1);

  return ret;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    print_help();
    return -1;
  }

  int port_num = atoi(argv[1]);

  int status = tcp_server_run(port_num, server_auth, traffic_gen_run, 1);
  traffic_gen_cleanup();

  return status;
}

