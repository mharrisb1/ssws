#ifndef LOG_H
#define LOG_H

typedef enum { TRACE, DEBUG, INFO, WARNING, ERROR } LogLevel;

extern LogLevel current_log_level;

void log_message(const LogLevel log_level, const char *format, ...);

#define trace(...)   log_message(TRACE, __VA_ARGS__)
#define debug(...)   log_message(DEBUG, __VA_ARGS__)
#define info(...)    log_message(INFO, __VA_ARGS__)
#define warning(...) log_message(WARNING, __VA_ARGS__)
#define error(...)   log_message(ERROR, __VA_ARGS__)
#endif
