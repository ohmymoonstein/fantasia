#include "util.hh"
#include <cstdarg>

std::string util_format(const char *format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
	va_end(args);
    buffer[sizeof(buffer) -1] = 0;
    return buffer;
}