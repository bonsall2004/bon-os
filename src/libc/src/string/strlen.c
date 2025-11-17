//
// Created by bonsall2004 on 16/11/2025.
//
#include <string.h>

size_t strlen(const char* s) {
  size_t len = 0;
  while (*s++)
    len++;
  return len;
}

