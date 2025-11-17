/*******************************************************************************
 * Project: BonOS
 * File: src/libc/src/string/strlen.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include <string.h>

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s++)
    len++;
  return len;
}
