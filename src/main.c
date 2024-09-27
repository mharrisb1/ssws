#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cli.h"
#include "config.h"
#include "log.h"
#include "net.h"
#include "server.h"
#include "sig.h"

int main(int argc, char **argv) {
  int                     client_fd;
  socklen_t               sin_size;
  struct sockaddr_storage their_addr;

  Config *config = Config_default();
  cli_parse(argc, argv, config);

  int sockfd = setup_server_socket(config);

  if (sockfd == -1) {
    exit(EXIT_FAILURE);
  }

  setup_sighandler();

  info("🍦 ssws started");
  info("Address:\thttp://%s:%s", config->host, config->port);
  info("Root:\t\t%s/%s", config->dir, config->html_index);

  while (1) {
    sin_size  = sizeof(their_addr);
    client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (client_fd == -1) {
      error("server: accept (%s)", strerror(errno));
      continue;
    }

    char dst[INET6_ADDRSTRLEN];
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              dst, sizeof(dst));
    debug("server: got connection from %s", dst);

    if (!fork()) {
      close(sockfd);
      handle_client(client_fd, config);
    }
    close(client_fd);
  }

  close(sockfd);
  free(config);
  return EXIT_SUCCESS;
}
