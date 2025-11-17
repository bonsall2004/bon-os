#pragma once
#include <stddef.h>
#include <stdint.h>

struct limine_framebuffer *get_framebuffer();

bool try_init_framebuffer();

void framebuffer_put_pixel(size_t x, size_t y, uint32_t color);
