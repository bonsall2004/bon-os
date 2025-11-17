/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/arch/x86/io.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include <arch/x86/io.h>

uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

void outb(uint16_t port, uint8_t val) { __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port)); }

void cpuid(uint32_t eax_in, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
  uint32_t a, b, c, d;
  __asm__ volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(eax_in));
  *eax = a;
  *ebx = b;
  *ecx = c;
  *edx = d;
}
