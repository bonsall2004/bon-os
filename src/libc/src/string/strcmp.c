//
// Created by bonsall2004 on 16/11/2025.
//
#include <string.h>

int strcmp(const char* a, const char* b) {
  while (*a && (*a == *b)) {
    a++; b++;
  }
  return *(unsigned char*)a - *(unsigned char*)b;
}
