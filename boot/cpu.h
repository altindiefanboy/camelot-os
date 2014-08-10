/*
 * cpu.h
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

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Halts the CPU.
 */
void cpu_halt(void);

/**
 * @brief Disables interrupts and halts the CPU, preventing further CPU
 * activity, as no interrupts can cause the CPU to halt at this point.
 */
void cpu_infinite_halt(void);

/**
 * @brief Gets the current state of the FLAGS register.
 *
 * @return Current state of the FLAGS register.
 */
uint32_t cpu_get_flags(void);

/**
 * @brief Checks FLAGS register to determine whether hardware interrupts are
 * enabled.
 *
 * @return State of CPU "IF" flag (true if set, false if cleared).
 */
bool cpu_check_interrupts(void);

/**
 * @brief Enables hardware interrupts.
 */
void cpu_enable_interrupts(void);

/**
 * @brief Disables hardware interrupts.
 */
void cpu_disable_interrupts(void);

#endif /* CPU_H_INCLUDED */
