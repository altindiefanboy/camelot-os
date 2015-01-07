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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>

/**
 * @brief Directly writes a string to stdout without formatting.
 *
 * @param str String to write. This should be null-terminated.
 * @param len Length of the string to write.
 */
void write(const char* str, size_t len)
{
    char backup_char = str[len];
    ((char*) str)[len] = 0;

    terminal_write_string(str);

    ((char*) str)[len] = backup_char;
}
