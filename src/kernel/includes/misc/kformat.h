/*******************************************************************************
 * Project: BonOS
 * File: src/kernel/include/misc/kformat.h
 * Author: Bonsall2004
 * Copyright © 2025 - 2025
 ******************************************************************************/

#pragma once
#include <stdarg.h>

static char *itoa_inner(long value, char *buf_end, int base, int uppercase);
char *kformat(const char *fmt, va_list args);
