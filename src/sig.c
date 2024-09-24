#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "sig_handler.h"

void sigchld_handler(int n) {
  (void)n;
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0) {
    // Child process reaped
  }
  errno = saved_errno;
}

void setup_sighandler() {
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    error("sigaction (%s)", strerror(errno));
    exit(EXIT_FAILURE);
  }
}
