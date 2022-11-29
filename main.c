#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "tcp_socket.h"
#include "user_options.h"
#include "socket_data.h"
#include "client.h"
#include "server.h"
#include "common.h"

int get_server_socket() {
    // connect
    int socketfd = tcp_socket_connect("punch.onlinewebshop.net", 80, 10);
    if(socketfd <= 0) {
        printf("Socket connect failed with code: %d", socketfd);
        return -1;
    }

    return socketfd;
}

socket_data get_invalid_socket_data() {
    socket_data peer_socket_data;
    peer_socket_data.port = -1;

    return peer_socket_data;
}

socket_data parse_server_response(const char* server_response, int peer_id) {
    socket_data data = get_invalid_socket_data();
    const char* header_body_separator = "\r\n\r\n";
    char* separator_position = strstr(server_response, header_body_separator);
    if(separator_position == NULL) {
        printf("Body separator not found!");   
        return data;
    }

    Print(server_response);
    printf("Server response length: %lu\r\n", strlen(server_response));

    Print("Starting actual parsing...");
    // parsing...
    // 3:111.222.33.44:5678
    char temp_client_id[10];
    char temp_port[10];
    const char seperator = ':';

    char* body_pointer = separator_position + strlen(header_body_separator);
    printf("Body pointer: %p\r\n", (void*)body_pointer);
    Print("Body:");
    Print(body_pointer);
    printf("Body length: %lu\r\n", strlen(body_pointer));
    while (TRUE)
    {
         Print("Iterating...");
        int client_id;
        int temp_length;
        // parse client id
        separator_position = strchr(body_pointer, seperator);
        memset(temp_client_id, 0, 10);
        memcpy(temp_client_id, body_pointer, separator_position - body_pointer);
        client_id = atoi(temp_client_id);
        body_pointer = separator_position + 1;
        Print("Client ID done.");

        // parse ip
        printf("Body pointer: %p\r\n", (void*)body_pointer);
        separator_position = strchr(body_pointer, (int)seperator);
        printf("Seperator position: %p\r\n", (void*)separator_position);
        memset(data.ip, 0, 16);
        Print("Memset done!");
        uint64_t sp = (uint64_t)separator_position;
        uint64_t bp = (uint64_t)body_pointer;
        uint64_t temp_length2 = sp - bp;
        printf("Temp length: %llu", temp_length2);
        memcpy(data.ip, body_pointer, temp_length2);
        body_pointer = separator_position + 1;
        Print("IP done.");

        // parse port
        separator_position = strstr(body_pointer, "\r\n");
        memset(temp_port, 0, 10);
        memcpy(temp_port, body_pointer, separator_position - body_pointer);
        data.port = atoi(temp_port);
        Print("Port done.");
        
        // skip newline
        body_pointer = separator_position + 2;

        // match client id
        if(client_id == peer_id) {
            Print("Match found!");
            return data;
        }

        printf("Remaining data: %lu", strlen(body_pointer));

        // breaking condition
        if(strlen(body_pointer) < 3) {
            Print("Breaking!");
            break;
        }
    }

    return data;
}



socket_data get_peer_socket_data(int server_socket, user_options options) {
    socket_data peer_socket_data = get_invalid_socket_data();
    
    int response = make_server_call(server_socket, options);
    if(response < 0) {
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

    return parse_server_response(buffer, options.peer_id);
}

user_options parse_user_options(int argc, char* argv[]) {
    int c;
    user_options options;
    options.peer_id = -1;
    options.my_id = -1;
    options.is_media_server = FALSE;

    while (((c = getopt(argc, argv, "sp:m:")) != -1))
    {
        switch (c)
        {
        case 'p':
            options.peer_id = atoi(optarg);
            break;
        case 'm':
            options.my_id = atoi(optarg);
            break;
        case 's':
            options.is_media_server = TRUE;
            break;
        default:
            break;
        }
    }

    return options;
}
// A
// A_PORT=50000
// B_PORT=51000
// To PHP Server:   S_IP=A_IP, S_PORT=A_PORT, D_IP=S_IP, D_PORT=S_PORT
// To B (client):   S_IP=A_IP, S_PORT=A_PORT, D_IP=B_IP, D_PORT=B_PORT
// From B (server): S_IP=B_IP, S_PORT=B_PORT, D_IP=A_IP, D_PORT=A_PORT



int main(int argc, char* argv[]) {

    user_options options = parse_user_options(argc, argv);
    if(options.peer_id < 0) {
        printf("No peer id given.\r\n");
        printf("Usage: main -p 3 -m 2 [s]\r\n");
        return -1;
    }
    printf("Peer id: %d\r\n", options.peer_id);

    if(options.my_id < 0) {
        printf("No my id given.\r\n");
        return -1;
    }
    printf("My id: %d\r\n", options.my_id);

    if(options.is_media_server) {
        printf("Media server mode.r\n");
    } else {
        printf("Client  mode.\r\n");
    }
    
    int server_socket = get_server_socket();
    Print("Got server socket.");

    if(options.is_media_server) {
        Print("Acting as a server");
        handle_as_server(server_socket, options);
    } else {
        Print("Acting as a client");
        socket_data media_server_data = get_peer_socket_data(server_socket, options);
        if(media_server_data.port < 0) {
            printf("Peer not found!\r\n");
            return -1;
        }
        Print("Got peer socket data");
        printf("Peer ip: %s, port: %d\r\n", media_server_data.ip, media_server_data.port);
        handle_as_client(server_socket, options, media_server_data);
    }

    Print("Closing socket");
    tcp_socket_close(server_socket);

    return 0;
}
