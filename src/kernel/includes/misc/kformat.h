#pragma once
#include <stdarg.h>

static char *itoa_inner(long value, char *buf_end, int base, int uppercase);
char *kformat(const char *fmt, va_list args);
