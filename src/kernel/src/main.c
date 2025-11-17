/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/main.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/
#include <arch/x86/exceptions/exceptions.h>
#include <arch/x86/gdt/gdt.h>
#include <arch/x86/idt/idt.h>
#include <arch/x86/io.h>
#include <font.h>
#include <framebuffer/framebuffer.h>
#include <serial/serial.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <framebuffer/logging.h>
#include "limine.h"

static void hcf(void) {
  for (;;) {
    asm("hlt");
  }
}

void kmain(void) {
  serial_init();

  if (!try_init_framebuffer()) {
    kprintf("Failed to initialize framebuffer!\r\n");
    use_serial_only = true;
  }
  if (!use_serial_only) {
    fb_clear(get_framebuffer(), 0x000000);
  }

  kprintf("In the kernel!!!\r\n");

  kprintf("[GDT]: Initialising...\r\n");
  gdt_init();
  kprintf("[GDT]: Initialised!\r\n");

  kprintf("[IDT]: Initialising...\r\n");
  idt_init();
  kprintf("[IDT]: Initialised!\r\n");

  kprintf("[Exceptions]: Initialising...\r\n");
  exception_init();
  kprintf("[Exceptions]: Initialised!\r\n");

  hcf();
}
