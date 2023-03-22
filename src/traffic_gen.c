#include "traffic_gen.h"

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "linked_list.h"

#define BUF_SIZE 65535  // max size tcp payload
typedef void (*sighandler_t)(int);

static linked_list* rates = NULL;
static int sec_passed = 1;
static int end_app = 0;
static int do_record = 0;
static int init = 0;
static char buf[BUF_SIZE] = {0};

static int compare_rates(const void* rate_1, const void* rate_2) {
  int val_r1 = *(const int*)rate_1;
  int val_r2 = *(const int*)rate_2;
  return val_r1 == val_r2;
}

static void process_alarm(int signum) { sec_passed = 1; }
static void process_sigint(int signum) { end_app = 1; }

static void record_traffic_rate(int sockfd) {
  static uint32_t bits_rcvd = 0;
  int counter_no_bytes = 0;

  while (!end_app) {
    if (sec_passed) {
      bits_rcvd *= 8;
      printf("- %.2lf Kbits/s, %.2lf Mbits/s\n", (double)bits_rcvd / 1000,
             (double)bits_rcvd / 1000000);
      linked_list_insert(rates, &bits_rcvd, sizeof(uint32_t));
      bits_rcvd = 0;
      sec_passed = 0;
      alarm(1);
    }

    int bytes = read(sockfd, &buf, sizeof(buf));
    if (!bytes) {
      if (++counter_no_bytes == 5) {
        end_app = 1;
      }
    } else
      counter_no_bytes = 0;

    bits_rcvd += bytes;
  }
}

void send_bytes(int sockfd) {
  while (!end_app) {
    write(sockfd, buf, sizeof(buf));
  }
}

void traffic_gen_run(int sockfd) {
  if (do_record) {
    record_traffic_rate(sockfd);
  } else {
    send_bytes(sockfd);
  }
}

void print_rates() {
  double avg = 0;
  int rates_sz = linked_list_size(rates);

  for (int i = rates_sz - 2; i >= 0; i--) {
    uint32_t rate = *(int*)linked_list_at(rates, i);
    //    double kbits = (double)rate / 1000;
    //  double mbits = (double)rate / 1000000;
    avg += rate;
    //    printf("Sec: %u, rate: %.2lf Kbits/s, %.2lf Mbits/s\n", rates_sz - i -
    //    1,
    //         kbits, mbits);
  }

  avg /= (rates_sz - 1);

  printf("\nAvg: %.2lf Kbits/s, %.2lf Mbits/s\n", avg / 1000, avg / 1000000);
}

void traffic_gen_cleanup() {
  if (do_record) {
    print_rates();
    linked_list_cleanup(&rates);
  }

  init = 0;
}

void traffic_gen_init(int _do_record) {
  if (init) return;

  do_record = _do_record;
  signal(SIGINT, process_sigint);

  if (do_record) {
    signal(SIGALRM, process_alarm);
    rates = linked_list_init(compare_rates, NULL);
  }

  init = 1;
}

