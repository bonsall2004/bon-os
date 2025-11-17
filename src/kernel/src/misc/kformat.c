#include <kformat.h>

#include "kformat.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

static char *itoa_inner(long value, char *buf_end, int base, int uppercase) {
  const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  unsigned long v = (value < 0 && base == 10) ? -value : (unsigned long) value;

  *buf_end = '\0';
  char *p = buf_end;

  do {
    *--p = digits[v % base];
    v /= base;
  } while (v);

  if (value < 0 && base == 10)
    *--p = '-';

  return p;
}

char *kformat(const char *fmt, va_list args) {
  static char out[1024];
  char *dst = out;

  while (*fmt && dst < out + sizeof(out) - 1) {

    if (*fmt != '%') {
      *dst++ = *fmt++;
      continue;
    }

    fmt++; // skip %

    char tmp[64];
    char *str;

    switch (*fmt) {
      case 'c':
        *dst++ = (char) va_arg(args, int);
        break;

      case 's':
        str = va_arg(args, char *);
        if (!str)
          str = "(null)";
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
        break;

      case 'd':
      case 'i': {
        int v = va_arg(args, int);
        str = itoa_inner(v, tmp + sizeof(tmp) - 1, 10, 0);
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
      } break;

      case 'u': {
        unsigned int v = va_arg(args, unsigned int);
        str = itoa_inner(v, tmp + sizeof(tmp) - 1, 10, 0);
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
      } break;

      case 'x': {
        unsigned int v = va_arg(args, unsigned int);
        str = itoa_inner(v, tmp + sizeof(tmp) - 1, 16, 0);
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
      } break;

      case 'X': {
        unsigned int v = va_arg(args, unsigned int);
        str = itoa_inner(v, tmp + sizeof(tmp) - 1, 16, 1);
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
      } break;

      case 'p': {
        uintptr_t v = (uintptr_t) va_arg(args, void *);
        *dst++ = '0';
        *dst++ = 'x';
        str = itoa_inner(v, tmp + sizeof(tmp) - 1, 16, 0);
        while (*str && dst < out + sizeof(out) - 1)
          *dst++ = *str++;
      } break;

      case '%':
        *dst++ = '%';
        break;

      default:
        *dst++ = '%';
        *dst++ = *fmt;
        break;
    }

    fmt++;
  }

  *dst = '\0';
  return out;
}
