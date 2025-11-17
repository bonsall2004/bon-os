//
// Created by bonsall2004 on 16/11/2025.
//

#pragma once

#include <stdint.h>

#ifdef __cplusplus

extern "C" {

#endif

typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

extern void gdt_load(gdt_ptr_t *ptr);

void gdt_init();

#ifdef __cplusplus
}
#endif
