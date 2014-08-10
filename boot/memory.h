/*
 * memory.h
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

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes memory for management.
 */
void memory_initialize(void);

/**
 * @brief Starting location in memory to begin allocation. Must be after
 * the end of kernel code in memory.
 */
extern void* memory_start;

/**
 * @brief Current location in memory to allocate memory at.
 */
extern void* memory_location;

/**
 * @brief Block alignment for memory management. Must be divisible by 2.
 */
extern size_t memory_block_alignment;

/**
 * @brief Block size for memory management.
 */
extern size_t memory_block_size;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MEMORY_H_INCLUDED */
