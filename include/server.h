#ifndef SERVER_H
#define SERVER_H

#include <netdb.h>

#include "config.h"

int setup_server_socket(Config *config);

void handle_client(int client_fd, Config *config);

#endif
