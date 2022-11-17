#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "tcp_socket.h"

struct socket_data {
    const char* ip;
    int port;
};

int get_server_socket() {
    // connect
    int socketfd = tcp_socket_connect("punch.onlinewebshop.net", 80, 10);
    if(socketfd <= 0) {
        printf("Socket connect failed with code: %d", socketfd);
        return -1;
    }

    return socketfd;
}

struct socket_data get_peer_socket_data(int server_socket, int peer_id) {
    struct  socket_data peer_socket_data;
    peer_socket_data.ip = "";
    peer_socket_data.port = -1;

    const char* http_body = "GET /?client-id=2 HTTP/1.1\r\nHost: punch.onlinewebshop.net \r\n\r\n";
    int bytes_written = tcp_socket_send(server_socket, http_body, strlen(http_body));
    if(bytes_written <= 0) {
        printf("Could not send data over socket.");
        return peer_socket_data;
    }

    int bufferLength = 512;
    char buffer[bufferLength];
    memset(buffer, 0, bufferLength);
    int bytes_read = tcp_socket_pull(server_socket, buffer, bufferLength, 10);
    if(bytes_read <= 0) {
        printf("Could not read data from socket.");
        return peer_socket_data;
    }

    printf("%s", buffer);

    return peer_socket_data;
}

int get_peer_id(int argc, char* argv[]) {
    int c;
    int peer_id;

    while (((c = getopt(argc, argv, "p:")) != -1))
    {
        switch (c)
        {
        case 'p':
            peer_id = atoi(optarg);
            return peer_id;        
        default:
            break;
        }
    }

    return -1;
}


int main(int argc, char* argv[]) {

    int peer_id = get_peer_id(argc, argv);
    if(peer_id < 0) {
        printf("No peer id given.");
        return -1;
    }

    printf("Peer id: %d", peer_id);
    return 0;

    int server_socket = get_server_socket();

    struct socket_data peer_socket_data = get_peer_socket_data(server_socket, peer_id);

    tcp_socket_close(server_socket);

    return 0;
}