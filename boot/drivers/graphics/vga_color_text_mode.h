/*
 * vga_color_text_mode.h
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

#ifndef VGA_COLOR_TEXT_MODE_H_INCLUDED
#define VGA_COLOR_TEXT_MODE_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <boot/drivers/terminal_driver.h>

/**
 * @brief Returns struct containing driver information.
 */
Terminal_Driver vga_color_text_mode_get_driver(void);

#endif /* VGA_COLOR_TEXT_MODE_H_INCLUDED */
