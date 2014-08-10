/*
 * terminal.h
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

#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <boot/drivers/terminal_driver.h>

/* Current row for writing. */
extern size_t terminal_row;

/* Current column for writing. */
extern size_t terminal_column;

/**
 * @brief Sets terminal driver.
 *
 * @param driver Driver to set as current terminal driver.
 */
void terminal_driver_set(Terminal_Driver driver);

/* Current driver being used. */
extern Terminal_Driver driver;

/* Current row for writing. */
extern size_t terminal_row;

/* Current column for writing. */
extern size_t terminal_column;

/**
 * @brief Initializes terminal for use.
 */
void (*terminal_initialize)(void);

/**
 * @brief Writes an character to the terminal.
 *
 * @param c Character to be written.
 * @param x Terminal column to write to.
 * @param y Terminal row to write to.
 */
void (*terminal_write_char)(const char c, size_t x, size_t y);

/**
 * @brief Copies entry from one location in the terminal to another.
 *
 * @param source_x Source column.
 * @param source_y Source row.
 * @param dest_x Destination column.
 * @param dest_y Destination row.
 */
void (*terminal_copy_entry)
	(size_t source_x, size_t source_y,
         size_t dest_x, size_t dest_y);

/**
 * @brief Clears the terminal.
 */
void (*terminal_clear_screen)(void);

/**
 * @brief Moves the cursor to the given location on the screen.
 *
 * @param x Column to move cursor to.
 * @param y Row to move cursor to.
 */
void (*terminal_move_cursor)(size_t x, size_t y);

/**
 * @brief Checks whether the cursor is visible.
 *
 * @return True if cursor is visible, false is cursor is hidden.
 */
bool (*terminal_check_cursor_visible)(void);

/**
 * @brief Hides the cursor, regardless of current visibility status.
 */
void (*terminal_hide_cursor)(void);

/**
 * @brief Shows the cursor, regardless of current status.
 */
void (*terminal_show_cursor)(void);

void terminal_scroll_up(void);

void terminal_handle_char(const char c);

void terminal_write_string(const char* str);

#endif /* TERMINAL_H_INCLUDED */
