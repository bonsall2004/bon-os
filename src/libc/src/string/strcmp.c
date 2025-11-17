/*******************************************************************************
* Project: BonOS
 * File: src/libc/src/string/strcmp.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include <string.h>

int strcmp(const char* a, const char* b) {
  while (*a && (*a == *b)) {
    a++; b++;
  }
  return *(unsigned char*)a - *(unsigned char*)b;
}
