/*
 * gdt.h
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

#include <boot/kernel/gdt/gdt.h>

void gdt_encode_descriptor(Gdt_Descriptor dest, Gdt_Descriptor_Source src)
{
    uint8_t* dest_8 = (uint8_t*) dest;

    uint16_t* dest_size = (uint16_t*) &dest_8[0];
    *dest_size = src.size;

    uint32_t* dest_offset = (uint32_t*) &dest_8[2];
    *dest_offset = src.offset;
}

void gdt_encode_entry(Gdt_Entry dest, Gdt_Entry_Source src)
{
    uint8_t* dest_8 = (uint8_t*) dest;

    uint16_t* dest_limit = (uint16_t*) &dest_8[0];
    *dest_limit = 0xFFFF & src.limit;

    uint8_t* dest_limit_2 = (uint8_t*) &dest_8[6];
    *dest_limit_2 = 0xF & (src.limit >> 16);

    uint16_t* dest_base = (uint16_t*) &dest_8[2];
    *dest_base = 0xFFFF & src.base;

    uint8_t* dest_base_2 = (uint8_t*) &dest_8[4];
    *dest_base_2 = 0xFF & (src.base >> 16);

    uint8_t* dest_base_3 = (uint8_t*) &dest_8[7];
    *dest_base_3 = 0xFF & (src.base >> 24);

    uint8_t t_access_byte = 0b00010000;
    t_access_byte |= 0b00000001 & src.accessed;
    t_access_byte |= 0b00000010 & (src.rw << 1);
    t_access_byte |= 0b00000100 & (src.dc << 2);
    t_access_byte |= 0b00001000 & (src.executable << 3);
    t_access_byte |= 0b01100000 & (src.privilege << 5);
    t_access_byte |= 0b10000000 & (src.present << 7);
    uint8_t* dest_access_byte = (uint8_t*) &dest_8[5];
    *dest_access_byte = t_access_byte;

    uint8_t t_flags = 0;
    t_flags |= 0b10000000 & (src.granularity << 7);
    t_flags |= 0b01000000 & (src.size << 6);
    dest_8[6] |= t_flags;
}
