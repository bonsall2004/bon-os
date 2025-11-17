/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/arch/x86/idt/idt.h
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IDT_ENTRIES 256
#define KERNEL_CS 0x08
#define DOUBLE_FAULT_IST 1

struct regs_state {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};


void register_interrupt_handler(int vector, void (*handler)(struct regs_state *, uint64_t vector, uint64_t err));

void idt_init(void);

#ifdef __cplusplus
}
#endif
