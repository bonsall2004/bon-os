//
// Created by bonsall2004 on 16/11/2025.
//

#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {



#endif

void *memcpy(void *dest, const void *src, size_t n);

void *memmove(void *dest, const void *src, size_t n);

void *memset(void *dest, int value, size_t n);

int memcmp(const void *a, const void *b, size_t n);

size_t strlen(const char *s);

char *strcpy(char *dest, const char *src);

int strcmp(const char *a, const char *b);

#ifdef __cplusplus
}
#endif
