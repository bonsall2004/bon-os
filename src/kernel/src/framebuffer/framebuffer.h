/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/framebuffer/framebuffer.h
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/
#pragma once
#include <stddef.h>
#include <stdint.h>

struct limine_framebuffer *get_framebuffer();

bool try_init_framebuffer();

void framebuffer_put_pixel(size_t x, size_t y, uint32_t color);
