/*
 * isr.s
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

.global isr_0
.global isr_1
.global isr_2
.global isr_3
.global isr_4
.global isr_5
.global isr_6
.global isr_7
.global isr_8
.global isr_9
.global isr_10
.global isr_11
.global isr_12
.global isr_13
.global isr_14
.global isr_15
.global isr_16
.global isr_17
.global isr_18
.global isr_19
.global isr_20
.global isr_21
.global isr_22
.global isr_23
.global isr_24
.global isr_25
.global isr_26
.global isr_27
.global isr_28
.global isr_29
.global isr_30
.global isr_31

.extern kernel_panic

.data

/* Exception error messages. */
Lisr_0_msg: .string "\nFATAL EXCEPTION: DIVIDE-BY-ZERO ERROR.\n"
Lisr_1_msg: .string "\nFATAL EXCEPTION: DEBUG EXCEPTION.\n"
Lisr_2_msg: .string "\nFATAL EXCEPTION: NON-MASKABLE INTERRUPT.\n"
Lisr_3_msg: .string "\nFATAL EXCEPTION: BREAKPOINT EXCEPTION.\n"
Lisr_4_msg: .string "\nFATAL EXCEPTION: OVERFLOW.\n"
Lisr_5_msg: .string "\nFATAL EXCEPTION: BOUND RANGE EXCEEDED.\n"
Lisr_6_msg: .string "\nFATAL EXCEPTION: INVALID OPCODE.\n"
Lisr_7_msg: .string "\nFATAL EXCEPTION: DEVICE NOT AVAILABLE.\n"
Lisr_8_msg: .string "\nFATAL EXCEPTION: DOUBLE FAULT.\n"
Lisr_9_msg: .string "\nFATAL EXCEPTION: COPROCESSOR SEGMENT OVERRUN.\n"
Lisr_10_msg: .string "\nFATAL EXCEPTION: INVALID TSS.\n"
Lisr_11_msg: .string "\nFATAL EXCEPTION: SEGMENT NOT PRESENT.\n"
Lisr_12_msg: .string "\nFATAL EXCEPTION: STACK-SEGMENT FAULT.\n"
Lisr_13_msg: .string "\nFATAL EXCEPTION: GENERAL PROTECTION FAULT.\n"
Lisr_14_msg: .string "\nFATAL EXCEPTION: PAGE FAULT.\n"
Lisr_15_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 15.\n"
Lisr_16_msg: .string "\nFATAL EXCEPTION: X87 FLOATING-POINT EXCEPTION.\n"
Lisr_17_msg: .string "\nFATAL EXCEPTION: ALIGNMENT CHECK.\n"
Lisr_18_msg: .string "\nFATAL EXCEPTION: MACHINE CHECK.\n"
Lisr_19_msg: .string "\nFATAL EXCEPTION: SIMD FLOATING-POINT EXCEPTION.\n"
Lisr_20_msg: .string "\nFATAL EXCEPTION: VIRTUALIZATION EXCEPTION.\n"
Lisr_21_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 21.\n"
Lisr_22_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 22.\n"
Lisr_23_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 23.\n"
Lisr_24_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 24.\n"
Lisr_25_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 25.\n"
Lisr_26_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 26.\n"
Lisr_27_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 27.\n"
Lisr_28_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 28.\n"
Lisr_29_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 29.\n"
Lisr_30_msg: .string "\nFATAL EXCEPTION: SECURITY EXCEPTION.\n"
Lisr_31_msg: .string "\nFATAL EXCEPTION: RESERVED INTERRUPT 31.\n"

.text

/* Handle Divide-By-Zero Error exception. */
.align 8
isr_0:
    pushal

    pushl $Lisr_0_msg
    call kernel_panic

/* Handle Debug exception. */
.align 8
isr_1:
    pushal

    pushl $Lisr_1_msg
    call kernel_panic

/* Handle Non-Maskable Interrupt exception. */
.align 8
isr_2:
    pushal

    pushl $Lisr_2_msg
    call kernel_panic

/* Handle Breakpoint exception. */
.align 8
isr_3:
    pushal

    pushl $Lisr_3_msg
    call kernel_panic

/* Handle Overflow exception. */
.align 8
isr_4:
    pushal

    pushl $Lisr_4_msg
    call kernel_panic

/* Handle Bound Range Exceeded exception. */
.align 8
isr_5:
    pushal

    pushl $Lisr_5_msg
    call kernel_panic

/* Handle Invalid Opcode exception. */
.align 8
isr_6:
    pushal

    pushl $Lisr_6_msg
    call kernel_panic

/* Handle Device Not Available exception. */
.align 8
isr_7:
    pushal

    pushl $Lisr_7_msg
    call kernel_panic

/* Handle Double Fault exception. */
.align 8
isr_8:
    pushal

    pushl $Lisr_8_msg
    call kernel_panic

/* Handle Coprocessor Segment Overrun exception. This should not actually
 * be called by the processor, because modern processors (486 processors+)
 * treat this as a General Protection Fault exception. The ISR is still here
 * just in case the kernel is ever run on an incompatible processor, to ease
 * debugging by providing an error message. */
.align 8
isr_9:
    pushal

    pushl $Lisr_9_msg
    call kernel_panic

/* Handles Invalid TSS exception. */
.align 8
isr_10:
    pushal

    pushl $Lisr_10_msg
    call kernel_panic

/* Handles Segment Not Present exception. */
.align 8
isr_11:
    pushal

    pushl $Lisr_11_msg
    call kernel_panic

/* Handles Stack-Segment Fault exception. */
.align 8
isr_12:
    pushal

    pushl $Lisr_12_msg
    call kernel_panic

/* Handles General Protection Fault exception. */
.align 8
isr_13:
    pushal

    pushl $Lisr_13_msg
    call kernel_panic

/* Handles Page Fault exception. */
.align 8
isr_14:
    pushal

    pushl $Lisr_14_msg
    call kernel_panic

/* Handles Reserved Interrupt 15 exception. */
.align 8
isr_15:
    pushal

    pushl $Lisr_15_msg
    call kernel_panic

/* Handles x87 Floating-Point Exception. */
.align 8
isr_16:
    pushal

    pushl $Lisr_16_msg
    call kernel_panic

/* Handles Alignment Check exception. */
.align 8
isr_17:
    pushal

    pushl $Lisr_17_msg
    call kernel_panic

/* Handles Machine Check exception. */
.align 8
isr_18:
    pushal

    pushl $Lisr_18_msg
    call kernel_panic

/* Handles SIMD Floating-Point Exception. */
.align 8
isr_19:
    pushal

    pushl $Lisr_19_msg
    call kernel_panic

/* Handles Virtualization Exception. */
.align 8
isr_20:
    pushal

    pushl $Lisr_20_msg
    call kernel_panic

/* Handles Reserved Interrupt 21 exception. */
.align 8
isr_21:
    pushal

    pushl $Lisr_21_msg
    call kernel_panic

/* Handles Reserved Interrupt 22 exception. */
.align 8
isr_22:
    pushal

    pushl $Lisr_22_msg
    call kernel_panic

/* Handles Reserved Interrupt 23 exception. */
.align 8
isr_23:
    pushal

    pushl $Lisr_23_msg
    call kernel_panic

/* Handles Reserved Interrupt 24 exception. */
.align 8
isr_24:
    pushal

    pushl $Lisr_24_msg
    call kernel_panic

/* Handles Reserved Interrupt 25 exception. */
.align 8
isr_25:
    pushal

    pushl $Lisr_25_msg
    call kernel_panic

/* Handles Reserved Interrupt 26 exception. */
.align 8
isr_26:
    pushal

    pushl $Lisr_26_msg
    call kernel_panic

/* Handles Reserved Interrupt 27 exception. */
.align 8
isr_27:
    pushal

    pushl $Lisr_27_msg
    call kernel_panic

/* Handles Reserved Interrupt 28 exception. */
.align 8
isr_28:
    pushal

    pushl $Lisr_28_msg
    call kernel_panic

/* Handles Reserved Interrupt 29 exception. */
.align 8
isr_29:
    pushal

    pushl $Lisr_29_msg
    call kernel_panic

/* Handles Security Exception. */
.align 8
isr_30:
    pushal

    pushl $Lisr_30_msg
    call kernel_panic

/* Handles Reserved Interrupt 31 exception. */
.align 8
isr_31:
    pushal

    pushl $Lisr_31_msg
    call kernel_panic
