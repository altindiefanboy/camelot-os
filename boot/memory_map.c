/*
 * memory_map.c
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

#include <boot/multiboot.h>
#include <boot/memory_map.h>

void multiboot_get_multiboot_info(multiboot_info_t* mbd)
{
	multiboot_info_structure.flags = mbd->flags;
	multiboot_info_structure.mem_lower = mbd->mem_lower;
	multiboot_info_structure.mem_upper = mbd->mem_upper;
	multiboot_info_structure.boot_device = mbd->boot_device;
	multiboot_info_structure.cmdline = mbd->cmdline;
	multiboot_info_structure.mods_count = mbd->mods_count;
	multiboot_info_structure.mods_addr = mbd->mods_addr;
	multiboot_info_structure.u = mbd->u;
	multiboot_info_structure.mmap_length = mbd->mmap_length;
	multiboot_info_structure.mmap_addr = mbd->mmap_addr;
}

multiboot_info_t multiboot_info_structure;
