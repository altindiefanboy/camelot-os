/*
 * terminal.c
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

#include <boot/drivers/terminal_driver.h>
#include <boot/ui/terminal.h>

Terminal_Driver terminal_driver;

size_t terminal_width;
size_t terminal_height;

void terminal_driver_set(Terminal_Driver driver)
{
	terminal_row = 0;
	terminal_column = 0;

	terminal_driver = driver;

	terminal_width = terminal_driver.terminal_width;
	terminal_height = terminal_driver.terminal_height;

	terminal_initialize = terminal_driver.terminal_initialize;
	terminal_write_char = terminal_driver.terminal_write_char;
	terminal_copy_entry = terminal_driver.terminal_copy_entry;
	terminal_clear_screen = terminal_driver.terminal_clear_screen;
	terminal_check_cursor_visible
		=
		terminal_driver.terminal_check_cursor_visible;
	terminal_move_cursor = terminal_driver.terminal_move_cursor;
	terminal_show_cursor = terminal_driver.terminal_show_cursor;
	terminal_hide_cursor = terminal_driver.terminal_hide_cursor;
}

size_t terminal_row;
size_t terminal_column;

void terminal_scroll_up(void)
{
	terminal_column = 0;
	for (size_t y = 0; y < (terminal_driver.terminal_height - 1); y++)
	{
		for (size_t x = 0; x < terminal_driver.terminal_width; x++)
		{
			terminal_copy_entry(x, y + 1, x, y);
		}
	}
	for ( size_t x = 0; x < terminal_driver.terminal_width; x++ )
	{
		terminal_write_char(' ', x, terminal_driver.terminal_height - 1);
	}
}

void terminal_handle_char(const char c)
{
	switch (c)
	{
	/* Handles newline character. */
	case '\n':
		if (terminal_row < (terminal_driver.terminal_height - 1))
		{
			terminal_row++;
			terminal_column = 0;
		}
		else
		{
			terminal_scroll_up();
		}
		break;
	/* Interprets as printable character. */
	default:
		terminal_write_char
			(c,
			terminal_column,
			terminal_row);
		if (terminal_column < (terminal_driver.terminal_width - 1))
		{
			terminal_column++;
		}
		else
		{
			if (terminal_row < (terminal_driver.terminal_height - 1))
			{
				terminal_row++;
				terminal_column = 0;
			}
			else
			{
				terminal_scroll_up();
			}
		}
		break;
	}
}

void terminal_write_string(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
	{
		terminal_handle_char(str[i]);
	}
}
