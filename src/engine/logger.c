#include <stdarg.h>
#include <stdio.h>
#include "color.h"
#include "logger.h"

static const char* get_level_str(int level) {
	switch(level) {
		case LOG_DEBUG:
			return "[" COLOR_BOLD COLOR_YELLOW "Debug" COLOR_RESET "] ";
		case LOG_ERROR:
			return "[" COLOR_BOLD COLOR_RED "Error" COLOR_RESET "] ";
		case LOG_WARNING:
			return "[" COLOR_BOLD COLOR_MAGENTA "Warning" COLOR_RESET "] ";
		case LOG_INFO:
			return "[" COLOR_BOLD COLOR_CYAN "Info" COLOR_RESET "] ";
	}
	return "";
}

void log_write(int level, const char* fmt, ...) {
#ifdef NDEBUG
	if(level == LOG_DEBUG)
		return;
#endif
	printf(get_level_str(level));

	va_list args;
	va_start(args, fmt);

	vprintf(fmt, args);

	va_end(args);

}
