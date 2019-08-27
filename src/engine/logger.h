#ifndef LOGGER_H
#define LOGGER_H

enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

void log_write(int level, const char* fmt, ...);

#endif
