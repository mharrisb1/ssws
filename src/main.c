#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cli.h"
#include "config.h"
#include "log.h"
#include "server.h"

#define BUFSIZE 1024

void sigchld_handler(int) {
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0) {
    // Child process reaped
  }
  errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char **argv) {
  int                     new_fd;
  socklen_t               sin_size;
  struct sigaction        sa;
  struct sockaddr_storage their_addr;

  char dst[INET6_ADDRSTRLEN];

  char buf[BUFSIZE];
  int  numbytes;

  Config *config = Config_default();

  cli_parse(argc, argv, config);
  int sockfd = new_server_socket(config);

  if (sockfd == -1) {
    exit(EXIT_FAILURE);
  }

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    error("sigaction (%s)", strerror(errno));
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  info("🍦 ssws started");
  info("Address:\thttp://%s:%s", config->host, config->port);
  info("Root:\t\t%s/%s", config->dir, config->html_index);

  while (1) {
    sin_size = sizeof(their_addr);
    new_fd   = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      error("server: accept (%s)", strerror(errno));
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              dst, sizeof(dst));
    debug("server: got connection from %s", dst);

    if (!fork()) {
      close(sockfd);

      numbytes = recv(new_fd, buf, BUFSIZE - 1, 0);
      if (numbytes == -1) {
        error("server: recv (%s)", strerror(errno));
        close(new_fd);
        exit(EXIT_FAILURE);
      }
      buf[numbytes] = '\0';

      debug("server: received request\n%s", buf);

      const char *response = "HTTP/1.1 200 OK\r\n"
                             "Content-Type: text/plain\r\n"
                             "Content-Length: 13\r\n"
                             "\r\n"
                             "Hello, world!";

      if (send(new_fd, response, strlen(response), 0) == -1) {
        error("server: send (%s)", strerror(errno));
      }

      close(new_fd);
      exit(EXIT_SUCCESS);
    }
    close(new_fd);
  }

  close(sockfd);
  free(config);
  return EXIT_SUCCESS;
}
