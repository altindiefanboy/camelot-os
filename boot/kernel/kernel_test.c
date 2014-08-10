/*
 * test.c
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

#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <stdlib.h>

#include <globals.h>
#include <boot/memory_map.h>
#include <boot/kernel/kernel.h>
#include <boot/kernel/kernel_test.h>
#include <boot/kernel/gdt/gdt.h>

static void test(void)
{
	char tchar_true = 't';
	char tchar_false = 'f';
	char* tret = malloc(sizeof(char) * 2);
	tret[0] = tchar_false;
	tret[1] = '\0';

	char* tstr = malloc(200);
	tstr = strcpy(tstr, "");
	terminal_write_string(tstr);
	if (1)
	{
		tret[0] = tchar_true;
	}

	char* tretstr = malloc(sizeof(char) * 200);
	tretstr = strcpy(tretstr, "\nTest returned: ");
	tretstr = strcat(tretstr, tret);
	tretstr = strcat(tretstr, "\n");
	terminal_write_string(tretstr);
}

static void test_end(void)
{
	terminal_write_string
	(
		"\nTEST COMPLETE\n"
	);
}

void kernel_test_start(void)
{
	terminal_write_string
	(
		"\nTESTING KERNEL\n"
	);

	test();
	test_end();
}
