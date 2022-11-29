//
//  client.c
//  hole-punch
//
//  Created by Md Abdullah Al Bakee on 29/11/2022.
//
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include "tcp_socket.h"
#include "user_options.h"
#include "socket_data.h"

#define TRUE 1
#define FALSE 0

#include "client.h"

void handle_as_client(int socketfd, user_options options, socket_data media_server_data) {
    int media_server_socketfd;
    // try connecting to the server
    while(TRUE) {
        printf("Connecting to media server: %s:%d\r\n", media_server_data.ip, media_server_data.port);
        media_server_socketfd = tcp_socket_connect(/*media_server_data.ip*/"127.0.0.1", media_server_data.port, 10);
        if(media_server_socketfd > 0) {
            break;
        }
    }

    // Give user prompt to type message
    char user_input[100];
    char server_response[100];
    while (TRUE)
    {
        memset(user_input, 0, 100);
        printf("Write your message: ");
        scanf("%s", user_input);
        int user_input_length = strlen(user_input);
        tcp_socket_send(media_server_socketfd, user_input, user_input_length);

        // print server response
        printf("Server: ");
        memset(server_response, 0, 100);
        int response_length = tcp_socket_pull(media_server_socketfd, server_response, 100, 10);
        printf("%s", server_response);
    }
}
