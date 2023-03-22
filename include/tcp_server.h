#ifndef TCP_SRV
#define TCP_SRV

// returns socket of call to socket
int tcp_server_init(int port);

// returns socket connection
int tcp_server_listen(int sockfd);
void tcp_server_cleanup(int sockfd);

// the argument times is how many times listen for
// a new connetion before ending it.
// auth_cb is a callback used to define a custom authentication
// between client and server.
int tcp_server_run(int port, int (*auth_cb)(int), void (*exec_cb)(int),
                   int times);
#endif
