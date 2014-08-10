/*
 * idt.h
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

#define IDT_DIVIDE_BY_ZERO_ERROR 0
#define IDT_DEBUG_EXCEPTION 1
#define IDT_NON_MASKABLE_INTERRUPT 2
#define IDT_BREAKPOINT_EXCEPTION 3
#define IDT_OVERFLOW_EXCEPTION 4
#define IDT_BOUND_RANGE_EXCEEDED 5
#define IDT_INVALID_OPCODE 6
#define IDT_DEVICE_NOT_AVAILABLE 7
#define IDT_DOUBLE_FAULT 8
#define IDT_COPROCESSOR_SEGMENT_OVERRUN 9
#define IDT_INVALID_TSS 10
#define IDT_SEGMENT_NOT_PRESENT 11
#define IDT_STACK_SEGMENT_FAULT 12
#define IDT_GENERAL_PROTECTION_FAULT 13
#define IDT_PAGE_FAULT 14
#define IDT_RESERVED_INTERRUPT_15 15
#define IDT_X87_FLOATING_POINT_EXCEPTION 16
#define IDT_ALIGNMENT_CHECK 17
#define IDT_MACHINE_CHECK 18
#define IDT_SIMD_FLOATING_POINT_EXCEPTION 19
#define IDT_VIRTUALIZATION_EXCEPTION 20
#define IDT_RESERVED_INTERRUPT_21 21
#define IDT_RESERVED_INTERRUPT_22 22
#define IDT_RESERVED_INTERRUPT_23 23
#define IDT_RESERVED_INTERRUPT_24 24
#define IDT_RESERVED_INTERRUPT_25 25
#define IDT_RESERVED_INTERRUPT_26 26
#define IDT_RESERVED_INTERRUPT_27 27
#define IDT_RESERVED_INTERRUPT_28 28
#define IDT_RESERVED_INTERRUPT_29 29
#define IDT_SECURITY_EXCEPTION 30
#define IDT_RESERVED_INTERRUPT_31 31

typedef struct Idt_Descriptor_Source
{
    /* Linear address of the IDT. */
    uint32_t offset;

    /* Size of the IDT - 1 byte. */
    uint16_t size;
} Idt_Descriptor_Source;

typedef enum Idt_Gate
{
    TASK_GATE_32 = 0b0101,
    INTERRUPT_GATE_16 = 0b0110,
    TRAP_GATE_16 = 0b0111,
    INTERRUPT_GATE_32 = 0b1110,
    TRAP_GATE_32 = 0b1111
} Idt_Gate;

typedef struct Idt_Entry_Source
{
    /* Linear address of the interrupt handler function. */
    uint32_t offset;

    /* Selector in the GDT to be used by this IDT gate. */
    uint16_t selector;

    /* Type of IDT gate. */
    Idt_Gate type;

    /* Storage segment. This should be 0 for interrupt gates. */
    uint8_t storage;

    /* Gate call protection. This specifies the minimum privilige level
     * (highest ring level) that may call this interrupt. This provides
     * protection from priviliged interrupts being called from userspace. */
    uint8_t privilege;

    /* If 1, this entry is enabled for interrupts. If 0, it is unused for
     * interrupts, but may be used for paging. */
    uint8_t present;
} Idt_Entry_Source;

/**
 * @brief Pointer to an IDT descriptor.
 */
typedef uint8_t Idt_Descriptor[6] __attribute__((aligned(8)));

/**
 * @brief Pointer to an IDT entry.
 */
typedef volatile uint8_t Idt_Entry[8] __attribute__((aligned(8)));

/* IDT descriptor. */
extern Idt_Descriptor idt_descriptor;

/* IDT descriptor source. */
extern Idt_Descriptor_Source idt_descriptor_src;

/* IDT table entries. */
extern Idt_Entry* idt_entry;

/* IDT table entries source. */
extern Idt_Entry_Source* idt_entry_src;

/**
 * @brief Encodes an IDT descriptor.
 *
 * @param dest Location of actual descriptor.
 * @param src Source to encode the actual descriptor with.
 */
void idt_encode_descriptor(Idt_Descriptor dest, Idt_Descriptor_Source src);

/**
 * @brief Encodes an IDT entry.
 *
 * @param dest Location of actual entry.
 * @param src Source to encode the actual entry with.
 */
void idt_encode_entry(Idt_Entry dest, Idt_Entry_Source src);

/**
 * @brief Loads an IDT descriptor.
 *
 * @param descriptor Descriptor to load.
 */
void idt_load_descriptor(Idt_Descriptor descriptor);
