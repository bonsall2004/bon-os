//
// Created by bonsall2004 on 16/11/2025.
//
#include <string.h>

char* strcpy(char* dest, const char* src) {
  char* r = dest;
  while ((*dest++ = *src++));
  return r;
}