/*
 * boot.c
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
#include <boot/memory_map.h>
#include <boot/multiboot.h>
#include <boot/drivers/graphics/vga_color_text_mode.h>
#include <boot/kernel/kernel.h>
#include <boot/ui/terminal.h>

void boot_main(multiboot_info_t* mbd, unsigned int magic)
{
    /* Makes sure interrupts are disabled, since the interrupt handler hasn't
     * been enabled yet. */
    cpu_disable_interrupts();

    /* Make permanent copy of memory map. This is done first to preserve
     * the integrity of the structure, preventing the memory manager from
     * allocating it's memory. */
    multiboot_get_multiboot_info(mbd);

    /* Initialize memory manager. */
    memory_initialize();

    /* Set terminal driver. */
    terminal_driver_set(vga_color_text_mode_get_driver());

    /* Initialize terminal. */
    terminal_initialize();

    terminal_write_string("Booted.\n");

    /* Call kernel. */
    kernel_main();
}
