#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
  char *dir;
  char *host;
  char *port;
  char *html_index;
  char *html_404;
  char *html_50x;
} Config;

Config *Config_default();
#endif
