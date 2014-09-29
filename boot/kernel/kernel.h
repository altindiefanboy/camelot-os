/*
 * kernel.h
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

#ifndef KERNEL_H_INCLUDED
#define KERNEL_HPP_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Kernel.
 */
void kernel_main(void);

/**
 * @brief Writes error message and then prevents further execution.
 *
 * @param str String containing error message to write.
 * @param len Length of the string. The string will be nulled at this
 * position by the function to enforce this length in the case of an
 * error.
 */
void kernel_panic(char* str, size_t len);

#endif /* KERNEL_HPP_INCLUDED */
