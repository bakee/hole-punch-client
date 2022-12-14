#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "tcp_socket.h"
#include "user_options.h"
#include "socket_data.h"

#include "common.h"
#include "server.h"

void handle_as_server(int socketfd, user_options options) {
    int response = keep_alive(socketfd, options);
    if(response < 0) {
        Print("Server request failed!");
        return;
    }
    
    Print("Server request successful!");
    
    int local_port = tcp_get_local_port(socketfd);
    // bind/listen
    int server_socketfd = tcp_socket_listen("0.0.0.0", local_port);
    printf("Listening on port: %d\r\n", local_port);

    if(server_socketfd < 0) {
        printf("Error in listening...\r\n");
        return;
    }

    printf("Listen complete!\r\n");

    if(!options.is_udp) {
        // accept
        int client_socketfd = -1;
        char remote_ip[20];
        int remote_port;
        while (TRUE) {
            Print("Waiting for a connection.");
            client_socketfd = tcp_socket_accept(server_socketfd, remote_ip, &remote_port, 5);
            keep_alive(socketfd, options);
        }
        if(client_socketfd < 0) {
            printf("Error occured while accepting a connection. %d\r\n", client_socketfd);
            return;
        }
    printf("Accepted a connection %d\r\n", client_socketfd);

    }
    // reply in a loop, break in special case
    const int read_buffer_length = 100;
    char read_buffer[read_buffer_length];
    char write_buffer[read_buffer_length];

    while(TRUE) {
        Print("Pulling from client socket");
        int read_length = tcp_socket_pull(client_socketfd, read_buffer, 5, 2);
        if(read_length <= 0) {
            Print("No data read");
            continue;
        }

        printf("Got data from client: %s\r\n", read_buffer);
        memcpy(write_buffer, read_buffer, read_length);
        to_upper_case(write_buffer, read_length);
        
        Print("Sending response to client");
        printf("Response: %s\r\n", write_buffer);
        tcp_socket_send(client_socketfd, write_buffer, read_length);
    }
}
