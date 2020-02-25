#ifndef LOGGER_H
#define LOGGER_H

enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

void engine_log_write(int level, const char *fmt, ...);
void engine_log_info(const char *fmt, ...);
void engine_log_warning(const char *fmt, ...);
void engine_log_debug(const char *fmt, ...);
void engine_log_error(const char *fmt, ...);

#define TCOLOR_MODE(x) "\x1b[" #x "m"
#define TCOLOR_RESET TCOLOR_MODE(0)
#define TCOLOR_BOLD TCOLOR_MODE(1)
#define TCOLOR_FAINT TCOLOR_MODE(2)
#define TCOLOR_ITALIC TCOLOR_MODE(3)
#define TCOLOR_BOLD_ITALIC TCOLOR_BOLD TCOLOR_ITALIC
#define TCOLOR_UNDERLINE TCOLOR_MODE(4)
#define TCOLOR_REVERSE TCOLOR_MODE(7)
#define TCOLOR_CROSSED_OUT TCOLOR_MODE(9)
#define TCOLOR_UNDERLINE_OFF TCOLOR_MODE(24)
#define TCOLOR_FRAMED TCOLOR_MODE(51)
#define TCOLOR_ENCIRCLED TCOLOR_MODE(52)
#define TCOLOR_OVERLINED TCOLOR_MODE(53)

#define TCOLOR_BASE(x) "\x1b[38;5;" #x "m"
#define TCOLOR_BASE_BG(x) "\x1b[48;5;" #x "m"
#define TCOLOR_RGB(r, g, b) "\x1b[38;2;" #r ";" #g ";" #b "m"
#define TCOLOR_RGB_BG(r, g, b) "\x1b[48;2;" #r ";" #g ";" #b "m"

#define TCOLOR_BLACK TCOLOR_BASE(0)
#define TCOLOR_RED TCOLOR_BASE(1)
#define TCOLOR_GREEN TCOLOR_BASE(2)
#define TCOLOR_YELLOW TCOLOR_BASE(3)
#define TCOLOR_BLUE TCOLOR_BASE(4)
#define TCOLOR_MAGENTA TCOLOR_BASE(5)
#define TCOLOR_CYAN TCOLOR_BASE(6)
#define TCOLOR_WHITE TCOLOR_BASE(7)

#define TCOLOR_BLACK_BG TCOLOR_BASE_BG(0)
#define TCOLOR_RED_BG TCOLOR_BASE_BG(1)
#define TCOLOR_GREEN_BG TCOLOR_BASE_BG(2)
#define TCOLOR_YELLOW_BG TCOLOR_BASE_BG(3)
#define TCOLOR_BLUE_BG TCOLOR_BASE_BG(4)
#define TCOLOR_MAGENTA_BG TCOLOR_BASE_BG(5)
#define TCOLOR_CYAN_BG TCOLOR_BASE_BG(6)
#define TCOLOR_WHITE_BG TCOLOR_BASE_BG(7)

#endif
