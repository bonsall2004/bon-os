/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/src/framebuffer/logging.c
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#include "logging.h"
#include <font.h>
#include <kformat.h>
#include <serial/serial.h>
#include <stdarg.h>
#include <string.h>

#include "framebuffer.h"

bool was_cleared = false;

void fb_scroll_lines(struct limine_framebuffer *fb, size_t lines, size_t scale, uint32_t clear_color) {
  if (!fb || lines == 0)
    return;

  size_t scroll_pixels = lines * (FONT_HEIGHT * scale);
  if (scroll_pixels >= fb->height) {
    /* Clear entire screen */
    fb_clear(fb, clear_color);
    return;
  }

  size_t bytes_per_scanline = fb->pitch;
  uint8_t *base = (uint8_t *) fb->address;
  size_t move_bytes = (fb->height - scroll_pixels) * bytes_per_scanline;

  /* Move framebuffer memory up */
  memmove(base, base + (scroll_pixels * bytes_per_scanline), move_bytes);

  /* Clear bottom area */
  size_t clear_y = fb->height - scroll_pixels;
  fb_draw_rectangle(fb, 0, clear_y, fb->width, scroll_pixels, clear_color);
}


void fb_draw_char_scale(struct limine_framebuffer *_framebuffer, size_t px, size_t py, char ch, uint32_t color,
                        size_t scale) {
  if (!_framebuffer)
    return;
  if (scale < 1)
    scale = 1;

  size_t fb_width = _framebuffer->width;
  size_t fb_height = _framebuffer->height;
  size_t pitch_pixels = _framebuffer->pitch / 4;
  volatile uint32_t *fb_ptr = (volatile uint32_t *) (uintptr_t) _framebuffer->address;

  unsigned char u = (unsigned char) ch;
  if (u >= 128)
    return; /* font only covers 0..127 */

  for (size_t ry = 0; ry < FONT_HEIGHT; ry++) {
    unsigned char row_byte = font_bin[u][ry];

    for (size_t bx = 0; bx < FONT_WIDTH; bx++) {
      if ((row_byte & (1u << (7 - bx))) == 0)
        continue; /* pixel off, skip */

      /* top-left of the scaled pixel block */
      size_t base_y = py + ry * scale;
      size_t base_x = px + bx * scale;

      /* vertical scaling: write `scale` rows */
      for (size_t sy = 0; sy < scale; sy++) {
        size_t y = base_y + sy;
        if (y >= fb_height)
          break;
        volatile uint32_t *row = fb_ptr + y * pitch_pixels;

        for (size_t sx = 0; sx < scale; sx++) {
          size_t x = base_x + sx;
          if (x >= fb_width)
            continue;
          row[x] = color;
        }
      }
    }
  }
}

void fb_draw_rectangle(struct limine_framebuffer *_framebuffer, const size_t x, const size_t y, const size_t w,
                       const size_t h, const uint32_t color) {
  if (!_framebuffer)
    return;
  uint32_t *fb_ptr = _framebuffer->address;
  size_t pitch_pixels = _framebuffer->pitch / 4;
  for (size_t dy = 0; dy < h; dy++) {
    for (size_t dx = 0; dx < w; dx++) {
      fb_ptr[(y + dy) * pitch_pixels + (dx + x)] = color;
    }
  }
}

void fb_clear(struct limine_framebuffer *_framebuffer, uint32_t color) {
  if (!_framebuffer || use_serial_only)
    return;
  uint32_t *fb_ptr = _framebuffer->address;
  size_t pitch_pixels = _framebuffer->pitch / 4;
  for (size_t y = 0; y < _framebuffer->height; y++) {
    for (size_t x = 0; x < _framebuffer->width; x++) {
      fb_ptr[y * pitch_pixels + x] = color;
    }
  }
  was_cleared = true;
}


void fb_puts_scale(struct limine_framebuffer *_framebuffer, size_t px, size_t py, const char *s, uint32_t color,
                   size_t scale) {
  if (!_framebuffer || !s)
    return;
  size_t x = px;
  while (*s) {
    fb_draw_char_scale(_framebuffer, x, py, *s, color, scale);
    x += FONT_WIDTH * scale;
    s++;
  }
}

void fb_draw_char(struct limine_framebuffer *_framebuffer, size_t px, size_t py, char ch, uint32_t color) {
  fb_draw_char_scale(_framebuffer, px, py, ch, color, 1);
}

void fb_puts(struct limine_framebuffer *_framebuffer, size_t px, size_t py, const char *s, uint32_t color) {
  fb_puts_scale(_framebuffer, px, py, s, color, 1);
}

static void reverse_buf(char *b, size_t len) {
  for (size_t i = 0, j = len ? len - 1 : 0; i < j; ++i, --j) {
    char t = b[i];
    b[i] = b[j];
    b[j] = t;
  }
}

static size_t utoa_base(unsigned long long val, unsigned int base, int uppercase, char *out, size_t out_sz) {
  static const char digits_l[] = "0123456789abcdef";
  static const char digits_u[] = "0123456789ABCDEF";
  const char *digits = uppercase ? digits_u : digits_l;

  if (out_sz == 0)
    return 0;
  if (val == 0) {
    if (out_sz > 1) {
      out[0] = '0';
      out[1] = '\0';
      return 1;
    } else {
      out[0] = '\0';
      return 0;
    }
  }

  size_t i = 0;
  while (val != 0 && i + 1 < out_sz) {
    out[i++] = digits[val % base];
    val /= base;
  }
  out[i] = '\0';
  reverse_buf(out, i);
  return i;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap) {
  if (!buf || size == 0)
    return 0;
  char *out = buf;
  char *end = buf + size;
  const char *p = fmt;

  while (*p && out < end - 1) {
    if (*p != '%') {
      *out++ = *p++;
      continue;
    }
    p++; /* skip '%' */
    if (*p == '%') {
      *out++ = '%';
      p++;
      continue;
    }

    switch (*p) {
      case 'c': {
        int ch = va_arg(ap, int);
        if (out < end - 1)
          *out++ = (char) ch;
        break;
      }
      case 's': {
        const char *s = va_arg(ap, const char *);
        if (!s)
          s = "(null)";
        while (*s && out < end - 1)
          *out++ = *s++;
        break;
      }
      case 'd': {
        int v = va_arg(ap, int);
        unsigned long long uv;
        if (v < 0) {
          if (out < end - 1)
            *out++ = '-';
          uv = (unsigned long long) (-(long long) v);
        } else
          uv = (unsigned long long) v;
        char tmp[32];
        size_t len = utoa_base(uv, 10, 0, tmp, sizeof(tmp));
        for (size_t i = 0; i < len && out < end - 1; ++i)
          *out++ = tmp[i];
        break;
      }
      case 'u': {
        unsigned int uv = va_arg(ap, unsigned int);
        char tmp[32];
        size_t len = utoa_base(uv, 10, 0, tmp, sizeof(tmp));
        for (size_t i = 0; i < len && out < end - 1; ++i)
          *out++ = tmp[i];
        break;
      }
      case 'x':
      case 'X': {
        unsigned int uv = va_arg(ap, unsigned int);
        char tmp[32];
        size_t len = utoa_base(uv, 16, (*p == 'X'), tmp, sizeof(tmp));
        for (size_t i = 0; i < len && out < end - 1; ++i)
          *out++ = tmp[i];
        break;
      }
      case 'p': {
        void *ptr = va_arg(ap, void *);
        unsigned long long uv = (uintptr_t) ptr;
        if (out < end - 1) {
          *out++ = '0';
          if (out < end - 1)
            *out++ = 'x';
        }
        char tmp[32];
        size_t len = utoa_base(uv, 16, 0, tmp, sizeof(tmp));
        for (size_t i = 0; i < len && out < end - 1; ++i)
          *out++ = tmp[i];
        break;
      }
      default:
        if (out < end - 1)
          *out++ = '%';
        if (out < end - 1)
          *out++ = *p;
        break;
    }
    p++;
  }

  *out = '\0';
  return (int) (out - buf);
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int r = vsnprintf(buf, size, fmt, ap);
  va_end(ap);
  return r;
}

void fb_printf(const char *fmt, ...) {
  if (!fmt || !get_framebuffer())
    return;


  va_list ap;
  va_start(ap, fmt);
  char *final = kformat(fmt, ap);
  va_end(ap);

  struct limine_framebuffer *fb = get_framebuffer();
  const size_t scale = (size_t) FONT_DEFAULT_SCALE;
  const uint32_t color = FONT_LOGGING_DEFAULT_COLOR;

  static size_t cur_x = FONT_LOGGING_DEFAULT_X;
  static size_t cur_y = FONT_LOGGING_DEFAULT_Y;

  if (was_cleared) {
    was_cleared = false;
    cur_x = FONT_LOGGING_DEFAULT_X;
    cur_y = FONT_LOGGING_DEFAULT_Y;
  }

  const size_t glyph_w = FONT_WIDTH * scale;
  const size_t glyph_h = (FONT_HEIGHT * scale) + 4;

  size_t bpp = 4;
  if (fb->width != 0) {
    bpp = fb->pitch / fb->width;
    if (bpp == 0)
      bpp = 4;
  }

  for (size_t i = 0; final[i] != '\0'; ++i) {
    char ch = final[i];

    if (ch == '\n') {
      cur_x = FONT_LOGGING_DEFAULT_X;
      cur_y += glyph_h;

      if (cur_y + glyph_h > fb->height) {
        fb_scroll_lines(fb, 1, scale, 0x00000000u);
        if (cur_y >= glyph_h)
          cur_y -= glyph_h + 2;
        else
          cur_y = 0;
      }
      continue;
    } else if (ch == '\r') {
      cur_x = FONT_LOGGING_DEFAULT_X;
      continue;
    } else if (ch == '\t') {
      size_t spaces = 4;
      cur_x += spaces * glyph_w;

      if (cur_x + glyph_w > fb->width) {
        cur_x = FONT_LOGGING_DEFAULT_X;
        cur_y += glyph_h;
      }
      continue;
    } else if (ch == '\b') {
      if (cur_x >= glyph_w) {
        cur_x -= glyph_w;
      } else {
        if (cur_y >= glyph_h) {
          cur_y -= glyph_h;
          cur_x = fb->width - glyph_w;
        } else {
          cur_x = FONT_LOGGING_DEFAULT_X;
        }
      }

      fb_draw_rectangle(fb, cur_x, cur_y, glyph_w, glyph_h, 0x00000000u);
      continue;
    }

    if (cur_x + glyph_w > fb->width) {
      cur_x = FONT_LOGGING_DEFAULT_X;
      cur_y += glyph_h;
    }

    if (cur_y + glyph_h > fb->height) {
      fb_scroll_lines(fb, 1, scale, 0x00000000u);
      if (cur_y >= glyph_h)
        cur_y -= glyph_h;
      else
        cur_y = 0;
    }

    fb_draw_char_scale(fb, cur_x, cur_y, ch, color, scale);
    cur_x += glyph_w;
  }
}

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  if (use_serial_only) {
    serial_printf(fmt, args);
  } else {
    fb_printf(fmt, args);
    serial_printf(fmt, args);
  }
  va_end(args);
}
