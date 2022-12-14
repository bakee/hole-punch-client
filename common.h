#ifndef common_h
#define common_h

#include "user_options.h"

#define TRUE 1
#define FALSE 0

void Print(const char* message);
void to_upper_case(char* str, const int str_length);
int keep_alive(int server_socket, user_options options);

#endif
