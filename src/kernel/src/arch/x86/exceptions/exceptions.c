//
// Created by bonsall2004 on 16/11/2025.
//
#include "exceptions.h"
#include "../idt/idt.h"
#include <stdint.h>
#include <stddef.h>
#include <serial/serial.h>

void halt(void) {
  asm volatile ("hlt");
}

#define MAX_EXCEPTIONS 32

static exception_handler_t handlers[MAX_EXCEPTIONS] = {0};

void exception_register_handler(uint8_t vector, exception_handler_t handler) {
  if (vector < MAX_EXCEPTIONS) {
    handlers[vector] = handler;
  }
}

void exception_common(uint64_t vector, uint64_t err, uint64_t *stack_frame) {
  cpu_regs_t regs = {0};
  cpu_frame_t frame = {0};

  frame.rip = stack_frame[0];
  frame.cs = stack_frame[1];
  frame.rflags = stack_frame[2];

  if (stack_frame[3] && stack_frame[4]) {
    frame.rsp = stack_frame[3];
    frame.ss = stack_frame[4];
  }

  if (vector < MAX_EXCEPTIONS && handlers[vector]) {
    handlers[vector](vector, err, &regs, &frame);
    return;
  }


  serial_printf(
    "=== Exception ===\n"
    "Vector: 0x%X\n"
    "Error code: 0x%X\n"
    "RIP: 0x%X\n"
    "CS: 0x%X\n"
    "RFLAGS: 0x%X\n"
    "RSP: 0x%X\n"
    "SS: 0x%X\n"
    "Halting.\n",
    (unsigned int) vector,
    (unsigned int) err,
    (unsigned int) frame.rip,
    (unsigned int) frame.cs,
    (unsigned int) frame.rflags,
    (unsigned int) frame.rsp,
    (unsigned int) frame.ss
  );

  halt();
}

void exception_init(void) {
  for (int i = 0; i < MAX_EXCEPTIONS; i++) {
    handlers[i] = NULL;
  }
}
