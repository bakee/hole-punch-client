#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tcp_socket.h"
#include "socket_data.h"

#include "common.h"
#include "server.h"

#include "common.h"

void Print(const char* message) {
    printf("%s\r\n", message);
}


void to_upper_case(char* str, const int str_length) {
    int i;
    for(i = 0; i < str_length; i++) {
        str[i] = (char) toupper(str[i]);
    }
}


int make_server_call(int server_socket, user_options options) {
    char http_body[200];
    memset(http_body, 0, 200);
    sprintf(http_body, "GET /?client-id=%d HTTP/1.1\r\nHost: punch.onlinewebshop.net \r\n\r\n", options.my_id);
    int bytes_written = tcp_socket_send(server_socket, http_body, strlen(http_body));
    if(bytes_written <= 0) {
        printf("Could not send data over socket.");
        return -1;
    }
    
    return 0;
}
