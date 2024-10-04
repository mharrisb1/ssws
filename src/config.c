#include <stdlib.h>

#include "config.h"

Config *Config_default() {
  Config *config     = (Config *)malloc(sizeof(Config));
  config->dir        = ".";
  config->host       = "0.0.0.0";
  config->port       = "8080";
  config->html_index = "index.html";
  config->html_404   = "404.html";
  config->html_50x   = "50x.html";
  return config;
}
