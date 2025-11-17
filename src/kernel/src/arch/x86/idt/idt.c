#include "idt.h"

#include <framebuffer/framebuffer.h>
#include <framebuffer/logging.h>

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

void KERN_PANIC(uint8_t vec, uint64_t err, struct registers_t *regs) {
  auto fb = get_framebuffer();
  if (fb)
    fb_clear(fb, 0x77000000u);

  if (vec < 32) {
    kprintf("KERNEL PANIC: %s", exception_messages[vec]);
  } else {
    kprintf("KERNEL PANIC: Unknown Exception");
  }
  kprintf("Exception Vector: 0x%X", vec);
  kprintf("Error Code: 0x%X", (unsigned int) err);
  kprintf("RIP: 0x%X", (unsigned int) regs->rip);
}
