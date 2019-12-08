#ifndef LOGGER_H
#define LOGGER_H

enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

void engine_log_write(int level, const char* fmt, ...);
void engine_log_info(const char *fmt, ...);
void engine_log_warning(const char *fmt, ...);
void engine_log_debug(const char *fmt, ...);
void engine_log_error(const char *fmt, ...);

#endif
