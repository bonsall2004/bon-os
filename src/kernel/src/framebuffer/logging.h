#pragma once
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#define FONT_BYTES_PER_CHAR (FONT_WIDTH * FONT_HEIGHT)
#define FONT_MAX_CHARS (sizeof(font_bin) / (FONT_BYTES_PER_CHAR))

#ifndef FONT_DEFAULT_SCALE
#define FONT_DEFAULT_SCALE 1
#endif

#ifndef FONT_LOGGING_DEFAULT_X
#define FONT_LOGGING_DEFAULT_X 10
#endif

#ifndef FONT_LOGGING_DEFAULT_Y
#define FONT_LOGGING_DEFAULT_Y 10
#endif

#ifndef FONT_LOGGING_DEFAULT_COLOR
#define FONT_LOGGING_DEFAULT_COLOR 0xFFFFFF
#endif

#define FB_PRINT_BUF_SIZE 4096

static bool use_serial_only = false;

void kprintf(const char *fmt, ...);

void fb_scroll_lines(struct limine_framebuffer *fb, size_t lines, size_t scale, uint32_t clear_color);

void fb_draw_char_scale(struct limine_framebuffer *_framebuffer, size_t px, size_t py, char ch, uint32_t color,
                        size_t scale);

void fb_draw_rectangle(struct limine_framebuffer *_framebuffer, const size_t x, const size_t y, const size_t w,
                       const size_t h, const uint32_t color);

void fb_clear(struct limine_framebuffer *_framebuffer, uint32_t color);


void fb_puts_scale(struct limine_framebuffer *_framebuffer, size_t px, size_t py, const char *s, uint32_t color,
                   size_t scale);

void fb_draw_char(struct limine_framebuffer *_framebuffer, size_t px, size_t py, char ch, uint32_t color);

void fb_puts(struct limine_framebuffer *_framebuffer, size_t px, size_t py, const char *s, uint32_t color);

void fb_printf(const char *s, ...);
