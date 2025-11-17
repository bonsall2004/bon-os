/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/arch/x86/gdt/gdt.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include "gdt.h"

static gdt_entry_t gdt[5];
static gdt_ptr_t gdt_ptr;

static void gdt_set_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
  gdt[idx].limit_low = limit & 0xFFFF;
  gdt[idx].base_low = base & 0xFFFF;
  gdt[idx].base_mid = (base >> 16) & 0xFF;
  gdt[idx].access = access;
  gdt[idx].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
  gdt[idx].base_high = (base >> 24) & 0xFF;
}

void gdt_init() {
  gdt_ptr.limit = sizeof(gdt) - 1;
  gdt_ptr.base = (uint64_t) &gdt;

  // 0: Null descriptor
  gdt_set_entry(0, 0, 0, 0, 0);

  // 1: Kernel Code (ring 0)
  gdt_set_entry(1, 0, 0x000FFFFF,
                0x9A, // present, ring0, executable, readable
                0x20 // Long mode segment
  );

  // 2: Kernel Data (ring 0)
  gdt_set_entry(2, 0, 0x000FFFFF,
                0x92, // present, ring0, writable
                0x00);

  // 3: User Code (ring 3)
  gdt_set_entry(3, 0, 0x000FFFFF,
                0xFA, // present, RING3, executable, readable
                0x20 // Long mode
  );

  // 4: User Data (ring 3)
  gdt_set_entry(4, 0, 0x000FFFFF,
                0xF2, // present, RING3, writable
                0x00);

  gdt_load(&gdt_ptr);
}
