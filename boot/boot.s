/*
 * boot.s
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

/* align loaded modules on page boundaries */
.set ALIGN, 1<<0

/* Declare constants used to define Multiboot header. */
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Define Multiboot header. */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Create a temporary stack, starting at the bottom. */
.section boot_stack
.align 16
stack_bottom:
    .skip (1024 * 16)    # 16 KiB
stack_top:

/* Create linker entry point '_start'. */
.section .text
.global _start
.type _start, @function
_start:
    /* Sets %esp to the top of the stack (the stack moves
     * downwards). */
    movl $stack_top, %esp

    /* Pushes %ebx to pass address for Multiboot memory map to the boot
     * procedure. */
    push %ebx

    /* Call boot procedure. */
    call boot_main

    /* Disable interrupts and halt computer if the program somehow returns. */
    cli
    hlt

    /* Jumps into infinite loop in case the halted cpu were to somehow
     * begin executing again. */
    0:
    1:
        jmp 0b

/* Set the size of the _start symbol to the current location '.' minus its start.
 * This is useful when debugging or when you implement call tracing. */
.size _start, . - _start
