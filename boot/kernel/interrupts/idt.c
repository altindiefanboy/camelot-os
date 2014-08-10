/*
 * idt.c
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

#include <boot/kernel/interrupts/idt.h>

void idt_encode_descriptor(Idt_Descriptor dest, Idt_Descriptor_Source src)
{
    uint8_t* dest_8 = (uint8_t*) dest;

    uint16_t* dest_size = (uint16_t*) &dest_8[0];
    *dest_size = src.size;

    uint32_t* dest_offset = (uint32_t*) &dest_8[2];
    *dest_offset = src.offset;
}

void idt_encode_entry(Idt_Entry dest, Idt_Entry_Source src)
{
    uint8_t* dest_8 = (uint8_t*) dest;

    uint32_t* dest_clear = (uint32_t*) &dest[0];
    dest_clear[0] = 0;
    dest_clear[1] = 0;

    uint16_t* dest_offset = (uint16_t*) &dest_8[0];
    *dest_offset = 0xFFFF & src.offset;

    uint16_t* dest_offset_2 = (uint16_t*) &dest_8[6];
    *dest_offset_2 = 0xFFFF & (src.offset >> 16);

    uint16_t* dest_selector = (uint16_t*) &dest_8[2];
    *dest_selector = 0xFFFF & src.selector;

    uint8_t* dest_type = (uint8_t*) &dest_8[5];
    *dest_type = 0xF & (src.type);

    uint8_t* dest_storage = (uint8_t*) &dest_8[5];
    *dest_storage |= 0b00010000 & (src.storage << 4);

    uint8_t* dest_privilege = (uint8_t*) &dest_8[5];
    *dest_privilege |= 0b01100000 & (src.privilege << 5);

    uint8_t* dest_present = (uint8_t*) &dest_8[5];
    *dest_present |= 0b10000000 & (src.present << 7);
}
