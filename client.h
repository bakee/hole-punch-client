//
//  client.h
//  hole-punch
//
//  Created by Md Abdullah Al Bakee on 29/11/2022.
//

#ifndef client_h
#define client_h

#include "user_options.h"
#include "socket_data.h"

void handle_as_client(int socketfd, user_options options, socket_data media_server_data);

#endif /* client_h */
