/*
 * port_io.h
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

#ifndef PORT_IO_H_INCLUDED
#define PORT_IO_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Sends an 8-bit unsigned integer to an I/O port.
 *
 * @param port Location of port.
 * @param value Value to send to port.
 */
inline void port_outb(uint16_t port, uint8_t value)
{
    asm volatile
    (
		"outb %[value], %[port]"
		: /* No outputs. */
		: [value] "a" (value), [port] "Nd" (port)
        : /* No clobbers. */
	);
}

/**
 * @brief Sends a 16-bit unsigned integer to an I/O port.
 *
 * @param port Location of port.
 * @param value Value to send to port.
 */
inline void port_outw(uint16_t port, uint16_t value)
{
    asm volatile
    (
		"outw %[value], %[port]"
		: /* No outputs. */
		: [value] "a" (value), [port] "Nd" (port)
        : /* No clobbers. */
	);
}

/**
 * @brief Sends a 32-bit unsigned integer to an I/O port.
 *
 * @param port Location of port.
 * @param value Value to send to port.
 */
inline void port_outl(uint16_t port, uint32_t value)
{
    asm volatile
    (
		"outl %[value], %[port]"
		:
		: [value] "a" (value), [port] "Nd" (port)
        : /* No clobbers. */
	);
}

/**
 * @brief Recieves an 8-bit unsigned integer from an I/O port.
 *
 * @param port Location of port.
 *
 * @return Value received from port.
 */
inline uint8_t port_inb(uint16_t port)
{
    uint8_t retval;

    asm volatile
    (
		"inb %[port], %[retval]"
		: [retval] "=a" (retval)
		: [port] "Nd" (port)
        : /* No clobbers. */
	);

    return (retval);
}

/**
 * @brief Recieves a 16-bit unsigned integer from an I/O port.
 *
 * @param port Location of port.
 *
 * @return Value received from port.
 */
inline uint16_t port_inw(uint16_t port)
{
    uint16_t retval;

    asm volatile
    (
		"inw %[port], %[retval]"
		: [retval] "=a" (retval)
		: [port] "Nd" (port)
        : /* No clobbers. */
	);

     return (retval);
}

/**
 * @brief Recieves a 32-bit unsigned integer from an I/O port.
 *
 * @param port Location of port in hexadecimal.
 *
 * @return Value received from port.
 */
inline uint32_t port_inl(uint16_t port)
{
    uint32_t retval;

    asm volatile
    (
		"inl %[port], %[retval]"
		: [retval] "=a" (retval)
		: [port] "Nd" (port)
        : /* No clobbers. */
	);

    return(retval);
}

/**
 * @brief Forces CPU to wait for I/O operation to complete before continuing
 * execution.
 *
 * @warning Not guaranteed to be successful. Currently implemented as a fix
 * found online that apparently works by causing the CPU to execute arbitrary
 * "jmp" instructions for the presumed time it takes to
 */
inline void port_io_wait(void)
{
    asm volatile
    (
		"outb $0, $0x80\n"
        : /* No outputs. */
        : /* No inputs. */
        : /* No clobbers. */
	);
}

#endif /* PORT_IO_H_INCLUDED */
