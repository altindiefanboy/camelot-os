/*
 * kernel_initialize.c
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

#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <boot/cpu.h>
#include <boot/memory.h>
#include <boot/drivers/terminal_driver.h>
#include <boot/drivers/graphics/vga_color_text_mode.h>
#include <boot/kernel/interrupts/idt.h>
#include <boot/kernel/interrupts/isr.h>
#include <boot/kernel/kernel.h>
#include <boot/kernel/gdt/gdt.h>
#include <boot/ui/terminal.h>

static void gdt_initialize(void);
static void idt_initialize(void);

void kernel_initialize(void)
{
    /* Interrupts must be disabled while setting up the GDT and IDT. */
    cpu_disable_interrupts();

    gdt_initialize();
    idt_initialize();
}

static void gdt_initialize(void)
{
    Gdt_Descriptor gdt_descriptor;
    Gdt_Descriptor_Source gdt_descriptor_src;

    Gdt_Entry gdt_entry[3];
    Gdt_Entry_Source gdt_entry_src[3];

    gdt_descriptor_src.offset = (uint32_t) &gdt_entry[0];
    gdt_descriptor_src.size = (8 * 3) - 1;
    gdt_encode_descriptor(gdt_descriptor, gdt_descriptor_src);

    gdt_entry_src[GDT_KERNEL_CODE].base = 0;
    gdt_entry_src[GDT_KERNEL_CODE].limit = 0xFFFFF;
    gdt_entry_src[GDT_KERNEL_CODE].present = 1;
    gdt_entry_src[GDT_KERNEL_CODE].privilege = 0;
    gdt_entry_src[GDT_KERNEL_CODE].executable = 1;
    gdt_entry_src[GDT_KERNEL_CODE].dc = 0;
    gdt_entry_src[GDT_KERNEL_CODE].rw = 0;
    gdt_entry_src[GDT_KERNEL_CODE].accessed = 0;
    gdt_entry_src[GDT_KERNEL_CODE].granularity = 1;
    gdt_entry_src[GDT_KERNEL_CODE].size = 1;
    gdt_encode_entry(
        gdt_entry[GDT_KERNEL_CODE],
        gdt_entry_src[GDT_KERNEL_CODE]);

    gdt_entry_src[GDT_KERNEL_DATA].base = 0;
    gdt_entry_src[GDT_KERNEL_DATA].limit = 0xFFFFF;
    gdt_entry_src[GDT_KERNEL_DATA].present = 1;
    gdt_entry_src[GDT_KERNEL_DATA].privilege = 0;
    gdt_entry_src[GDT_KERNEL_DATA].executable = 0;
    gdt_entry_src[GDT_KERNEL_DATA].dc = 0;
    gdt_entry_src[GDT_KERNEL_DATA].rw = 1;
    gdt_entry_src[GDT_KERNEL_DATA].accessed = 0;
    gdt_entry_src[GDT_KERNEL_DATA].granularity = 1;
    gdt_entry_src[GDT_KERNEL_DATA].size = 1;
    gdt_encode_entry(
        gdt_entry[GDT_KERNEL_DATA],
        gdt_entry_src[GDT_KERNEL_DATA]);

    gdt_load_descriptor(gdt_descriptor);

    gdt_load_selectors(GDT_KERNEL_CODE_SELECTOR, GDT_KERNEL_DATA_SELECTOR);
}

static void idt_initialize(void)
{
    Idt_Descriptor idt_descriptor;
    Idt_Descriptor_Source idt_descriptor_src;

    Idt_Entry idt_entry[256];
    Idt_Entry_Source idt_entry_src[256];

    idt_descriptor_src.offset = (uint32_t) &idt_entry[0];
    idt_descriptor_src.size = (8 * 256) - 1;
    idt_encode_descriptor(idt_descriptor, idt_descriptor_src);

    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].offset = (uint32_t) &isr_0;
    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].storage = 0;
    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].privilege = 0;
    idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR].present = 1;
    idt_encode_entry(
        idt_entry[IDT_DIVIDE_BY_ZERO_ERROR],
        idt_entry_src[IDT_DIVIDE_BY_ZERO_ERROR]);

    idt_entry_src[IDT_DEBUG_EXCEPTION].offset = (uint32_t) &isr_1;
    idt_entry_src[IDT_DEBUG_EXCEPTION].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_DEBUG_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_DEBUG_EXCEPTION].storage = 0;
    idt_entry_src[IDT_DEBUG_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_DEBUG_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_DEBUG_EXCEPTION],
        idt_entry_src[IDT_DEBUG_EXCEPTION]);

    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].offset = (uint32_t) &isr_2;
    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].storage = 0;
    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].privilege = 0;
    idt_entry_src[IDT_NON_MASKABLE_INTERRUPT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_NON_MASKABLE_INTERRUPT],
        idt_entry_src[IDT_NON_MASKABLE_INTERRUPT]);

    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].offset = (uint32_t) &isr_3;
    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].storage = 0;
    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_BREAKPOINT_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_BREAKPOINT_EXCEPTION],
        idt_entry_src[IDT_BREAKPOINT_EXCEPTION]);

    idt_entry_src[IDT_OVERFLOW_EXCEPTION].offset = (uint32_t) &isr_4;
    idt_entry_src[IDT_OVERFLOW_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_OVERFLOW_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_OVERFLOW_EXCEPTION].storage = 0;
    idt_entry_src[IDT_OVERFLOW_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_OVERFLOW_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_OVERFLOW_EXCEPTION],
        idt_entry_src[IDT_OVERFLOW_EXCEPTION]);

    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].offset = (uint32_t) &isr_5;
    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].storage = 0;
    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].privilege = 0;
    idt_entry_src[IDT_BOUND_RANGE_EXCEEDED].present = 1;
    idt_encode_entry(
        idt_entry[IDT_BOUND_RANGE_EXCEEDED],
        idt_entry_src[IDT_BOUND_RANGE_EXCEEDED]);

    idt_entry_src[IDT_INVALID_OPCODE].offset = (uint32_t) &isr_6;
    idt_entry_src[IDT_INVALID_OPCODE].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_INVALID_OPCODE].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_INVALID_OPCODE].storage = 0;
    idt_entry_src[IDT_INVALID_OPCODE].privilege = 0;
    idt_entry_src[IDT_INVALID_OPCODE].present = 1;
    idt_encode_entry(
        idt_entry[IDT_INVALID_OPCODE],
        idt_entry_src[IDT_INVALID_OPCODE]);

    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].offset = (uint32_t) &isr_7;
    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].storage = 0;
    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].privilege = 0;
    idt_entry_src[IDT_DEVICE_NOT_AVAILABLE].present = 1;
    idt_encode_entry(
        idt_entry[IDT_DEVICE_NOT_AVAILABLE],
        idt_entry_src[IDT_DEVICE_NOT_AVAILABLE]);

    idt_entry_src[IDT_DOUBLE_FAULT].offset = (uint32_t) &isr_8;
    idt_entry_src[IDT_DOUBLE_FAULT].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_DOUBLE_FAULT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_DOUBLE_FAULT].storage = 0;
    idt_entry_src[IDT_DOUBLE_FAULT].privilege = 0;
    idt_entry_src[IDT_DOUBLE_FAULT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_DOUBLE_FAULT],
        idt_entry_src[IDT_DOUBLE_FAULT]);

    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].offset = (uint32_t) &isr_9;
    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].storage = 0;
    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].privilege = 0;
    idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN].present = 1;
    idt_encode_entry(
        idt_entry[IDT_COPROCESSOR_SEGMENT_OVERRUN],
        idt_entry_src[IDT_COPROCESSOR_SEGMENT_OVERRUN]);

    idt_entry_src[IDT_INVALID_TSS].offset = (uint32_t) &isr_10;
    idt_entry_src[IDT_INVALID_TSS].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_INVALID_TSS].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_INVALID_TSS].storage = 0;
    idt_entry_src[IDT_INVALID_TSS].privilege = 0;
    idt_entry_src[IDT_INVALID_TSS].present = 1;
    idt_encode_entry(
        idt_entry[IDT_INVALID_TSS],
        idt_entry_src[IDT_INVALID_TSS]);

    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].offset = (uint32_t) &isr_11;
    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].storage = 0;
    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].privilege = 0;
    idt_entry_src[IDT_SEGMENT_NOT_PRESENT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_SEGMENT_NOT_PRESENT],
        idt_entry_src[IDT_SEGMENT_NOT_PRESENT]);

    idt_entry_src[IDT_STACK_SEGMENT_FAULT].offset = (uint32_t) &isr_12;
    idt_entry_src[IDT_STACK_SEGMENT_FAULT].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_STACK_SEGMENT_FAULT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_STACK_SEGMENT_FAULT].storage = 0;
    idt_entry_src[IDT_STACK_SEGMENT_FAULT].privilege = 0;
    idt_entry_src[IDT_STACK_SEGMENT_FAULT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_STACK_SEGMENT_FAULT],
        idt_entry_src[IDT_STACK_SEGMENT_FAULT]);

    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].offset = (uint32_t) &isr_13;
    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].storage = 0;
    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].privilege = 0;
    idt_entry_src[IDT_GENERAL_PROTECTION_FAULT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_GENERAL_PROTECTION_FAULT],
        idt_entry_src[IDT_GENERAL_PROTECTION_FAULT]);

    idt_entry_src[IDT_PAGE_FAULT].offset = (uint32_t) &isr_14;
    idt_entry_src[IDT_PAGE_FAULT].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_PAGE_FAULT].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_PAGE_FAULT].storage = 0;
    idt_entry_src[IDT_PAGE_FAULT].privilege = 0;
    idt_entry_src[IDT_PAGE_FAULT].present = 1;
    idt_encode_entry(
        idt_entry[IDT_PAGE_FAULT],
        idt_entry_src[IDT_PAGE_FAULT]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_15].offset = (uint32_t) &isr_15;
    idt_entry_src[IDT_RESERVED_INTERRUPT_15].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_15].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_15].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_15].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_15].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_15],
        idt_entry_src[IDT_RESERVED_INTERRUPT_15]);

    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].offset
        = (uint32_t) &isr_16;
    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].storage = 0;
    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_X87_FLOATING_POINT_EXCEPTION],
        idt_entry_src[IDT_X87_FLOATING_POINT_EXCEPTION]);

    idt_entry_src[IDT_ALIGNMENT_CHECK].offset = (uint32_t) &isr_17;
    idt_entry_src[IDT_ALIGNMENT_CHECK].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_ALIGNMENT_CHECK].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_ALIGNMENT_CHECK].storage = 0;
    idt_entry_src[IDT_ALIGNMENT_CHECK].privilege = 0;
    idt_entry_src[IDT_ALIGNMENT_CHECK].present = 1;
    idt_encode_entry(
        idt_entry[IDT_ALIGNMENT_CHECK],
        idt_entry_src[IDT_ALIGNMENT_CHECK]);

    idt_entry_src[IDT_MACHINE_CHECK].offset = (uint32_t) &isr_18;
    idt_entry_src[IDT_MACHINE_CHECK].selector = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_MACHINE_CHECK].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_MACHINE_CHECK].storage = 0;
    idt_entry_src[IDT_MACHINE_CHECK].privilege = 0;
    idt_entry_src[IDT_MACHINE_CHECK].present = 1;
    idt_encode_entry(
        idt_entry[IDT_MACHINE_CHECK],
        idt_entry_src[IDT_MACHINE_CHECK]);

    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].offset
        = (uint32_t) &isr_19;
    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].storage = 0;
    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_SIMD_FLOATING_POINT_EXCEPTION],
        idt_entry_src[IDT_SIMD_FLOATING_POINT_EXCEPTION]);

    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].offset = (uint32_t) &isr_20;
    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].storage = 0;
    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_VIRTUALIZATION_EXCEPTION],
        idt_entry_src[IDT_VIRTUALIZATION_EXCEPTION]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_21].offset = (uint32_t) &isr_21;
    idt_entry_src[IDT_RESERVED_INTERRUPT_21].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_21].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_21].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_21].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_21].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_21],
        idt_entry_src[IDT_RESERVED_INTERRUPT_21]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_22].offset = (uint32_t) &isr_22;
    idt_entry_src[IDT_RESERVED_INTERRUPT_22].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_22].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_22].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_22].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_22].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_22],
        idt_entry_src[IDT_RESERVED_INTERRUPT_22]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_23].offset = (uint32_t) &isr_23;
    idt_entry_src[IDT_RESERVED_INTERRUPT_23].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_23].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_23].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_23].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_23].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_23],
        idt_entry_src[IDT_RESERVED_INTERRUPT_23]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_24].offset = (uint32_t) &isr_24;
    idt_entry_src[IDT_RESERVED_INTERRUPT_24].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_24].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_24].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_24].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_24].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_24],
        idt_entry_src[IDT_RESERVED_INTERRUPT_24]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_25].offset = (uint32_t) &isr_25;
    idt_entry_src[IDT_RESERVED_INTERRUPT_25].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_25].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_25].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_25].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_25].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_25],
        idt_entry_src[IDT_RESERVED_INTERRUPT_25]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_26].offset = (uint32_t) &isr_26;
    idt_entry_src[IDT_RESERVED_INTERRUPT_26].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_26].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_26].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_26].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_26].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_26],
        idt_entry_src[IDT_RESERVED_INTERRUPT_26]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_27].offset = (uint32_t) &isr_27;
    idt_entry_src[IDT_RESERVED_INTERRUPT_27].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_27].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_27].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_27].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_27].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_27],
        idt_entry_src[IDT_RESERVED_INTERRUPT_27]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_28].offset = (uint32_t) &isr_28;
    idt_entry_src[IDT_RESERVED_INTERRUPT_28].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_28].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_28].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_28].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_28].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_28],
        idt_entry_src[IDT_RESERVED_INTERRUPT_28]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_29].offset = (uint32_t) &isr_29;
    idt_entry_src[IDT_RESERVED_INTERRUPT_29].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_29].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_29].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_29].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_29].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_29],
        idt_entry_src[IDT_RESERVED_INTERRUPT_29]);

    idt_entry_src[IDT_SECURITY_EXCEPTION].offset = (uint32_t) &isr_30;
    idt_entry_src[IDT_SECURITY_EXCEPTION].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_SECURITY_EXCEPTION].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_SECURITY_EXCEPTION].storage = 0;
    idt_entry_src[IDT_SECURITY_EXCEPTION].privilege = 0;
    idt_entry_src[IDT_SECURITY_EXCEPTION].present = 1;
    idt_encode_entry(
        idt_entry[IDT_SECURITY_EXCEPTION],
        idt_entry_src[IDT_SECURITY_EXCEPTION]);

    idt_entry_src[IDT_RESERVED_INTERRUPT_31].offset = (uint32_t) &isr_31;
    idt_entry_src[IDT_RESERVED_INTERRUPT_31].selector
        = GDT_KERNEL_CODE_SELECTOR;
    idt_entry_src[IDT_RESERVED_INTERRUPT_31].type = INTERRUPT_GATE_32;
    idt_entry_src[IDT_RESERVED_INTERRUPT_31].storage = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_31].privilege = 0;
    idt_entry_src[IDT_RESERVED_INTERRUPT_31].present = 1;
    idt_encode_entry(
        idt_entry[IDT_RESERVED_INTERRUPT_31],
        idt_entry_src[IDT_RESERVED_INTERRUPT_31]);

    idt_load_descriptor(idt_descriptor);
}
