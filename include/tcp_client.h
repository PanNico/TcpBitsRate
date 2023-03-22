#ifndef TCP_CLIENT
#define TCP_CLIENT

// returns socket of call to socket
int tcp_client_init(const char* srv_addr, int srv_port);

// returns socket connection
int tcp_client_connect(int sockfd);
void tcp_client_cleanup(int sockfd);
// auth_cb is a callback used to define a custom authentication
// between client and server.
int tcp_client_run(const char* srv_addr, int srv_port, int (*auth_cb)(int),
                   void (*exec_cb)(int));

#endif
