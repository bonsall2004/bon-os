/*******************************************************************************
 * Project: BonOS
 * File: src/libc/src/string/strcpy.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include <string.h>

char *strcpy(char *dest, const char *src) {
  char *r = dest;
  while ((*dest++ = *src++))
    ;
  return r;
}
