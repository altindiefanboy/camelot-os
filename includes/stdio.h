/*
 * stdio.h
 *
 * Copyright 2014 Seth Nils <altindiefanboy@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef _STDIO_H_INCLUDED
#define _STDIO_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @note "stdio.h" cannot be properly and fully implemented until filesystem
 * abstractions have been implemented.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Directly writes a string to stdout without formatting.
 *
 * @param str String to write. This should be null-terminated.
 * @param len Length of the string to write.
 */
void write(const char* str, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* _STDIO_H_INCLUDED */
