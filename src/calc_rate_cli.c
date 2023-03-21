#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcp_client.h"
#include "traffic_gen.h"

void print_help() {
  printf("./calc_rate_cli <download | upload> <port> <srv_addr>\n");
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Invalid number of arguments\n");
    print_help();
    return -1;
  }

  if (strcmp(argv[1], "upload") == 0) {
    traffic_gen_init(0);
  } else if (strcmp(argv[1], "download") == 0) {
    traffic_gen_init(1);
  } else {
    print_help();
    return -2;
  }

  int port_num = atoi(argv[2]);
  int status = tcp_client_run(argv[3], port_num, traffic_gen_run);
  traffic_gen_cleanup();

  return status;
}

