#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "log.h"
#include "server.h"

#define BACKLOG 10
#define BUFSIZE 4096

int setup_server_socket(Config *config) {
  int             status, sockfd = -1;
  struct addrinfo hints, *res, *p;
  const int       yes = 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = AI_NUMERICHOST;

  status = getaddrinfo(config->host, config->port, &hints, &res);
  if (status != 0) {
    error("server: getaddrinfo (%s)", gai_strerror(status));
    return sockfd;
  }

  for (p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      error("server: socket (%s)", strerror(errno));
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      error("server: setsockopt (%s)", strerror(errno));
      close(sockfd);
      sockfd = -1;
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      error("server: bind (%s)", strerror(errno));
      close(sockfd);
      sockfd = -1;
      continue;
    }

    break;
  }

  freeaddrinfo(res);

  if (sockfd == -1) {
    error("server: failed to bind");
    return sockfd;
  }

  if (listen(sockfd, BACKLOG) == -1) {
    error("server: listen (%s)", strerror(errno));
    close(sockfd);
    sockfd = -1;
    return sockfd;
  }

  return sockfd;
}

void handle_client(int client_fd, Config *conf) {
  char buf[BUFSIZE];
  int  nbytes = recv(client_fd, buf, BUFSIZE - 1, 0);
  if (nbytes == -1) {
    error("server: recv (%s)", strerror(errno));
    close(client_fd);
    exit(EXIT_FAILURE);
  }
  buf[nbytes] = '\0';

  debug("server: received request\n%s", buf);

  char response[BUFSIZE] = "HTTP/1.1 200 OK\n\n";

  char *method = strtok(buf, " ");
  if (strcmp(method, "GET") == 0) {
    char *path       = strtok(NULL, " ");
    char  fpath[100] = "";
    strcpy(fpath, conf->dir);
    strcat(fpath, path);
    if (strcmp(path, "/") == 0) {
      strcat(fpath, conf->html_index);
    }

    FILE *file = fopen(fpath, "rb");

    if (file != NULL) {
      if (fseek(file, 0, SEEK_END) == 0) {
        long filesize = ftell(file);
        rewind(file);
        char *fbuf = malloc(filesize + 1);
        fread(fbuf, 1, filesize, file);
        fbuf[filesize] = '\0';
        strcat(response, fbuf);
        free(fbuf);
      }
    }
    fclose(file);
    info("%s %s", method, path);
  }

  if (send(client_fd, response, strlen(response), 0) == -1) {
    error("server: send (%s)", strerror(errno));
  }

  close(client_fd);
  exit(EXIT_SUCCESS);
}
