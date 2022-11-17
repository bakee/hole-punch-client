// #include <errno.h>
// #include <stdio.h>

// #include <stdlib.h>
// #include <string.h>

// #include <arpa/inet.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <sys/stat.h>
// #include <sys/time.h>

// #include <dirent.h>
// #include <netdb.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <signal.h>
// #include <sys/select.h>

void tcp_socket_set_block(int socket, int on);

int tcp_socket_connect(const char *host, int port, int timeout);

int tcp_socket_close(int socketfd);

int tcp_socket_pull(int socketfd, char *data, int len, int timeout_sec);

int tcp_socket_send(int socketfd, const char *data, int len);

//return socket fd
int tcp_socket_listen(const char *address, int port);

//return client socket fd
int tcp_socket_accept(int onsocketfd, char *remoteip, int *remoteport, int timeouts);

//return socket port
int get_tcp_socket_port(int socketfd);