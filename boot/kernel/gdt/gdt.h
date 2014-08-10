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

#define GDT_KERNEL_CODE 1
#define GDT_KERNEL_DATA 2

#define GDT_KERNEL_CODE_SELECTOR 8
#define GDT_KERNEL_DATA_SELECTOR 16

typedef struct Gdt_Descriptor_Source
{
    /* Linear address of the GDT. */
    uint32_t offset;

    /* Size of the GDT - 1. */
    uint16_t size;
} Gdt_Descriptor_Source;

typedef struct Gdt_Entry_Source
{
    /* The linear address of where the segment begins, taking paging
     * into account. */
    uint32_t base;

    /* The maximum addressable unit for the segment. Unit is of a
     * size specified by the granularity bit of the GDT entry. */
    uint32_t limit;

    /* This must equal 1 for all valid selectors. */
    uint8_t present;

    /* The ring level for the entry. 0 specifies the highest
     * ring (ex. kernel). 3 specifies the lowest ring (ex. userspace). */
    uint8_t privilege;

    /* Whether code in the segment can be executed. 0 specifies
     * that the segment is a data segment. 1 specifies that the segment is a
     * code segment. */
    uint8_t executable;

    /* For a data segment, whether the segment grows up (0) or down
     * (1).
     * For a code segment, 1 means that the segment can be executed from
     * an equal or lower privilige level (lower in this sense means a higher
     * ring). The privilege field represents the highest privilege level
     * (lowest ring) that can execute, or far jump to, code in this
     * segment. */
    uint8_t dc;

    /* For code selectors, whether read access is allowed (1 if allowed).
     * For data selectors, whether write access is allowed (1 if allowed).
     * Write access is NEVER allowed for code segments.
     * Read access is ALWAYS allowed for data segments. */
    uint8_t rw;

    /* This should usually not be set manually. The CPU will set this field
     * if the segment is accessed. */
    uint8_t accessed;

    /* This determines the block size of the limit field of the GDT entry.
     * 0 specifies 1 B blocks (byte granularity). 1 specifies 4 KiB blocks
     * (page granularity). */
    uint8_t granularity;

    /* This determines the selector size. 0 specifies 16-bit protected
     * mode. 1 specifies 32-bit protected mode. */
    uint8_t size;
} Gdt_Entry_Source;

/**
 * @brief Pointer to a GDT descriptor.
 */
typedef uint8_t Gdt_Descriptor[6] __attribute__((aligned(8)));

/**
 * @brief Pointer to a GDT entry.
 */
typedef volatile uint8_t Gdt_Entry[8] __attribute__((aligned(8)));

/* GDT descriptor. */
extern Gdt_Descriptor gdt_descriptor;

/* GDT table entries. */
extern Gdt_Entry* gdt_entry;

/**
 * @brief Encodes a GDT descriptor.
 *
 * @param dest Location of actual descriptor.
 * @param src Source to encode the actual descriptor with.
 */
void gdt_encode_descriptor(Gdt_Descriptor dest, Gdt_Descriptor_Source src);

/**
 * @brief Encodes a GDT entry.
 *
 * @param dest Location of actual entry.
 * @param src Source to encode the actual entry with.
 */
void gdt_encode_entry(Gdt_Entry dest, Gdt_Entry_Source src);

/**
 * @brief Loads a GDT descriptor.
 *
 * @param descriptor Descriptor to load.
 */
extern void gdt_load_descriptor(Gdt_Descriptor descriptor);

/**
 * @brief Loads selectors into segment registers.
 *
 * @param code Code selector. This is the offset in bytes, from the beginning
 * of the GDT table, of the GDT entry describing the data segments.
 * @param data Data selector. This is the offset in bytes, from the beginning
 * of the GDT table, of the GDT entry describing the data segments.
 */
extern void gdt_load_selectors(uint16_t code, uint16_t data);
