/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/arch/x86/idt/idt.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include "idt.h"

#include <arch/x86/io.h>
#include <framebuffer/framebuffer.h>
#include <framebuffer/logging.h>

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t ist;
  uint8_t type_attr;
  uint16_t offset_mid;
  uint32_t offset_high;
  uint32_t zero;
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_descr;

extern void idt_load(void *idt_ptr);
extern void isr_common_entry(void);

const char *exception_messages[32] = {"Division By Zero",
                                      "Debug",
                                      "Non Maskable Interrupt",
                                      "Breakpoint",
                                      "Into Detected Overflow",
                                      "Out of Bounds",
                                      "Invalid Opcode",
                                      "No Coprocessor",
                                      "Double Fault",
                                      "Coprocessor Segment Overrun",
                                      "Bad TSS",
                                      "Segment Not Present",
                                      "Stack Fault",
                                      "General Protection Fault",
                                      "Page Fault",
                                      "Unknown Interrupt",
                                      "Coprocessor Fault",
                                      "Alignment Check",
                                      "Machine Check",
                                      "SIMD Floating-Point Exception",
                                      "Virtualization Exception",
                                      "Control Protection Exception",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved"};

typedef void (*intr_handler_t)(struct regs_state *, uint64_t vector, uint64_t err);
static intr_handler_t handlers[IDT_ENTRIES];

void register_interrupt_handler(int vector, void (*handler)(struct regs_state *, uint64_t, uint64_t)) {
  if (vector >= 0 && vector < IDT_ENTRIES)
    handlers[vector] = handler;
}

static void set_idt_entry(int vec, void *isr, uint8_t ist, uint8_t type_attr) {
  const uint64_t isr_addr = (uint64_t) isr;
  idt[vec].offset_low = (uint16_t) (isr_addr & 0xFFFF);
  idt[vec].selector = KERNEL_CS;
  idt[vec].ist = ist & 0x7;
  idt[vec].type_attr = type_attr;
  idt[vec].offset_mid = (uint16_t) ((isr_addr >> 16) & 0xFFFF);
  idt[vec].offset_high = (uint32_t) ((isr_addr >> 32) & 0xFFFFFFFF);
  idt[vec].zero = 0;
}

static void pic_remap(void) {
  uint8_t a1 = inb(0x21);
  uint8_t a2 = inb(0xA1);

  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20); // master offset = 0x20 (32)
  outb(0xA1, 0x28); // slave offset  = 0x28 (40)
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, a1);
  outb(0xA1, a2);
}

void isr_handler_c(struct regs_state *regs, uint64_t vector_ptr, uint64_t dummy) {
  uint64_t *vec_ptr = (uint64_t *) vector_ptr;
  uint64_t vector = vec_ptr[0];
  uint64_t err = vec_ptr[1];

  if (vector < IDT_ENTRIES && handlers[vector]) {
    handlers[vector](regs, vector, err);
  } else {
    (void) regs;
    (void) err;
    (void) vector;
    for (;;)
      __asm__ volatile("hlt");
  }
}

void idt_init(void) {
  pic_remap();

  for (int i = 0; i < IDT_ENTRIES; ++i) {
    extern void isr_0(void);
  }

  for (int i = 0; i < IDT_ENTRIES; ++i) {
    extern void *isr_table[];
    uint8_t ist = 0;
    uint8_t type_attr = 0x8E;
    if (i == 8)
      ist = DOUBLE_FAULT_IST;
    set_idt_entry(i, isr_table[i], ist, type_attr);
  }

  idt_descr.limit = sizeof(idt) - 1;
  idt_descr.base = (uint64_t) &idt;
  idt_load(&idt_descr);
}
