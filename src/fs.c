#include <stdio.h>

#include "fs.h"

int readlines(FILE *fp, char *buf) {
  if (fseek(fp, 0, SEEK_END) == 0) {
    long fsize = ftell(fp);
    rewind(fp);
    fread(buf, 1, fsize, fp);
    buf[fsize] = '\0';
    return 0;
  }
  return 1;
}
