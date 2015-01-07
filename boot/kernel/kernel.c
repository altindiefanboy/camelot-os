/*
 * kernel.c
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <boot/cpu.h>
#include <boot/memory.h>
#include <boot/port_io.h>
#include <boot/kernel/kernel.h>
#include <boot/kernel/kernel_initialize.h>
#include <boot/ui/terminal.h>

#ifdef TEST
#include <boot/kernel/kernel_test.h>
#endif /* TEST */

void kernel_main(void)
{
    terminal_write_string("Kernel booted.\n");

    /* Initializes kernel. This must be executed before any other kernel
     * operations. */
    kernel_initialize();

    char str_1[] = "Kernel initialized.\n";
    write(str_1, strlen(str_1));

    #ifdef TEST
    kernel_test_start();
    #endif /* TEST */

    char tstr[] = "this is a string\n";
    write(tstr, strlen(tstr));

    terminal_write_string("Kernel stopped successfully.\n");
}

void kernel_panic(char* str, size_t len)
{
    /* Ensures that the panic message is null-terminated. */
    str[len] = '\0';

    /* Prints error message. */
    write(str, len);

    /* Prevents further execution. */
    asm volatile
    (
        "cli\n"
        "hlt\n"
        "0:\n"
        "jmp 0b"
        : /* No outputs. */
        : /* No inputs. */
        : "cc"
    );
}
