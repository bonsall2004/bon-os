//
// Created by bonsall2004 on 16/11/2025.
//

#pragma once

#include <stdint.h>

typedef struct cpu_regs {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
  uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
} cpu_regs_t;

typedef struct cpu_frame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} cpu_frame_t;

typedef void (*exception_handler_t)(uint64_t vector, uint64_t err, cpu_regs_t *regs, cpu_frame_t *frame);

void exception_register_handler(uint8_t vector, exception_handler_t handler);

void exception_common(uint64_t vector, uint64_t err, uint64_t *stack_frame);

void exception_init(void);
