/*******************************************************************************
 * Project: BonOS
 * File: src/libc/src/memory/memset.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/
#include <string.h>

void *memset(void *s, int c, size_t n) {
  uint8_t *p = (uint8_t *) s;

  for (size_t i = 0; i < n; i++) {
    p[i] = (uint8_t) c;
  }

  return s;
}
