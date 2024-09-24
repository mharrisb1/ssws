#include <stdarg.h>
#include <stdio.h>

#include "log.h"

#define BOLD      "\e[1m"
#define UNDERLINE "\e[4m"
#define RESET     "\e[0m"

#define RED    "\e[31m"
#define GREEN  "\e[32m"
#define YELLOW "\e[33m"
#define BLUE   "\e[34m"
#define CYAN   "\e[36m"

static char *LEVEL_NAME[]  = {"TRACE", "DEBUG", "INFO ", "WARN ", "ERROR"};
static char *LEVEL_COLOR[] = {CYAN, BLUE, GREEN, YELLOW, RED};

LogLevel current_log_level = TRACE;

void log_message(const LogLevel log_level, const char *format, ...) {
  if (log_level < current_log_level) {
    return;
  }

  char *name  = LEVEL_NAME[log_level];
  char *color = LEVEL_COLOR[log_level];

  va_list args;
  va_start(args, format);

  FILE *output;

  if (log_level == ERROR) {
    output = stderr;
    fprintf(output, "[%s%s%s%s] ", BOLD, color, name, RESET);
  } else {
    output = stdout;
    fprintf(output, "[%s%s%s] ", color, name, RESET);
  }
  vfprintf(output, format, args);
  fprintf(output, "\n");

  va_end(args);
}
