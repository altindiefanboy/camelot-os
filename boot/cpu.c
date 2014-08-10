/*
 * cpu.c
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

#include <boot/cpu.h>

void cpu_halt(void)
{
    asm volatile
    (
		"hlt\n"
        : /* No outputs. */
        : /* No inputs. */
        : /* No clobbers. */
	);
}

void cpu_infinite_halt(void)
{
    cpu_disable_interrupts();
    cpu_halt();
}

uint32_t cpu_get_flags(void)
{
    uint32_t flags;
    asm
	(
		"pushfl\n"
		"popl %%eax\n"
        "movl %%eax, %[flags]\n"
        "pushl %%eax\n"
        "popfl\n"
		: [flags] "=g" (flags)
        : /* No inputs. */
        : /* No clobbers. */
	);
	return (flags);
}

bool cpu_check_interrupts(void)
{
    bool retval;

    uint32_t flags;
    flags = cpu_get_flags();
    retval = (flags & (1 << 9));

    return (retval);
}

void cpu_enable_interrupts(void)
{
    asm volatile
    (
		"sti\n"
        : /* No outputs. */
        : /* No inputs. */
        : "cc"
	);
}

void cpu_disable_interrupts(void)
{
    asm volatile
    (
		"cli\n"
        : /* No outputs. */
        : /* No inputs. */
        : "cc"
    );
}
