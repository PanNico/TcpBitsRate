#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_actors.h"
#include "tcp_client.h"
#include "traffic_gen.h"

static char selected_flow[SELECTED_FLOW_SZ] = {0};

void print_help() {
  printf("./calc_rate_cli <download | upload> <port> <srv_addr>\n");
}

int client_auth(int sockfd) {
  int bytes_written = 0;

  while ((bytes_written = write(sockfd, selected_flow + bytes_written,
                                SELECTED_FLOW_SZ - bytes_written)) <
         SELECTED_FLOW_SZ)
    ;

  uint8_t ret = 1;

  if (read(sockfd, &ret, sizeof(uint8_t))) return ret;

  return 1;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Invalid number of arguments\n");
    print_help();
    return -1;
  }

  if (strcmp(argv[1], "upload") == 0) {
    traffic_gen_init(0);
    strcpy(selected_flow, "download");
  } else if (strcmp(argv[1], "download") == 0) {
    traffic_gen_init(1);
    strcpy(selected_flow, "upload");
  } else {
    print_help();
    return -2;
  }

  int port_num = atoi(argv[2]);
  int status = tcp_client_run(argv[3], port_num, client_auth, traffic_gen_run);
  traffic_gen_cleanup();

  return status;
}

