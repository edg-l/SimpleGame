#ifndef LOGGER_H
#define LOGGER_H

enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

void log_write(int level, const char* fmt, ...);
void log_info(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_debug(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif
