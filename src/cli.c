#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "config.h"
#include "log.h"

#define OPT_INDEX_HTML 1000
#define OPT_404_HTML   1001
#define OPT_50x_HTML   1002

void cli_parse(int argc, char **argv, Config *config) {
  int opt;

  static struct option longopts[] = {
      {"dir", optional_argument, 0, 'd'},
      {"host", optional_argument, 0, 'a'},
      {"port", optional_argument, 0, 'p'},
      {"index", optional_argument, 0, OPT_INDEX_HTML},
      {"404", optional_argument, 0, OPT_404_HTML},
      {"50x", optional_argument, 0, OPT_50x_HTML},
      {"verbose", no_argument, 0, 'v'},
      {"quiet", no_argument, 0, 'q'},
      {"help", no_argument, 0, 'h'},
  };

  const char *optstr = "hd:a:p:vq";

  while ((opt = getopt_long(argc, argv, optstr, longopts, NULL)) != -1) {
    switch (opt) {
      case 'h':
        printf("🍦 Super simple web server\n");
        printf("\n");
        printf("Usage: %s [OPTIONS]\n", argv[0]);
        printf("\n");
        printf("Options:\n");
        printf("  -d, --dir   <PATH>\tRoot directory\t[default: .]\n");
        printf("  -a, --host  <ADDR>\tHost address\t[default: 0.0.0.0]\n");
        printf("  -p, --port  <PORT>\tHost port\t[default: 8080]\n");
        printf("      --index <PATH>\tRoot HTML file\t[default: index.html]\n");
        printf("      --404   <PATH>\t404 HTML file\t[default: 404.html]\n");
        printf("      --50x   <PATH>\t50x HTML file\t[default: 50x.html]\n");
        printf("  -v, --verbose\t\tIncrease logging verbosity\n");
        printf("  -q, --quiet\t\tDecrease logging verbosity\n");
        printf("  -h, --help\t\tPrint this message\n");
        exit(EXIT_SUCCESS);
      case 'd':
        if (optarg) {
          config->dir = optarg;
        }
        break;
      case 'a':
        if (optarg) {
          config->host = optarg;
        }
        break;
      case 'p':
        if (optarg) {
          config->port = optarg;
        }
        break;
      case OPT_INDEX_HTML:
        if (optarg) {
          config->html_index = optarg;
        }
        break;
      case OPT_404_HTML:
        if (optarg) {
          config->html_404 = optarg;
        }
        break;
      case OPT_50x_HTML:
        if (optarg) {
          config->html_50x = optarg;
        }
        break;
      case 'v':
        if (current_log_level > TRACE) {
          current_log_level--;
        }
        break;
      case 'q':
        if (current_log_level < ERROR) {
          current_log_level++;
        }
        break;
      case '?':
        exit(EXIT_FAILURE);
      default:
        abort();
    }
  }
}
