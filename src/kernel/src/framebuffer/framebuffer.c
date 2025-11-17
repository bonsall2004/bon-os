/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/framebuffer/framebuffer.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/
#include "framebuffer.h"
#include <limine.h>
#include <stdint.h>

__attribute__((used,
               section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] =
    LIMINE_BASE_REVISION(4);


static struct limine_framebuffer *framebuffer;
struct limine_framebuffer *get_framebuffer() {
  if (!framebuffer)
    framebuffer = framebuffer_request.response->framebuffers[0];
  return framebuffer;
}

bool try_init_framebuffer() {
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
    return false;
  }
  framebuffer = framebuffer_request.response->framebuffers[0];
  if (framebuffer == NULL) {
    return false;
  }

  return true;
}

void framebuffer_put_pixel(size_t x, size_t y, uint32_t color) {
  uint32_t *fb_ptr = framebuffer->address;

  fb_ptr[y + (x * framebuffer->pitch / 4)] = color;
}
