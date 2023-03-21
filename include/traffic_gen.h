#ifndef TRAFFIC_GEN_H
#define TRAFFIC_GEN_H

void traffic_gen_init(int _do_record);
void traffic_gen_run(int sockfd);
void traffic_gen_cleanup();

#endif
