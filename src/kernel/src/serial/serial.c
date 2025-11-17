/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/serial/serial.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include "serial.h"

#include <arch/x86/io.h>
#include <kformat.h>

#define COM1_PORT 0x3F8

void serial_init() {
  outb(0x3F8 + 1, 0x00); // disable interrupts
  outb(0x3F8 + 3, 0x80); // enable DLAB
  outb(0x3F8 + 0, 0x03); // set baud rate divisor (lo)
  outb(0x3F8 + 1, 0x00); // set baud rate divisor (hi)
  outb(0x3F8 + 3, 0x03); // 8 bits, no parity, 1 stop bit
  outb(0x3F8 + 2, 0xC7); // enable FIFO
}


void serial_write_char(char c) {
  while (!(inb(COM1_PORT + 5) & 0x20))
    ;
  outb(COM1_PORT, c);
}

void serial_write_string(const char *str) {
  while (*str) {
    serial_write_char(*str++);
  }
}

void serial_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  const char *final = kformat(fmt, args);
  serial_write_string(final);

  va_end(args);
}
