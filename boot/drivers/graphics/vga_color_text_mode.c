/*
 * vga_color_text_mode.c
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

#include <boot/port_io.h>
#include <boot/drivers/terminal_driver.h>
#include <boot/drivers/graphics/vga_color_text_mode.h>

/**
 * @brief Max visible terminal width.
 */
#define VGA_WIDTH 80

/**
 * @brief Max visible terminal height.
 */
#define VGA_HEIGHT 25

/**
 * @brief VGA color text mode buffer location.
 */
#define VGA_COLOR_TEXT_MODE_BUFFER 0xB8000

/**
 * @brief VGA controller command port.
 */
#define VGA_CRTC_PORT 0x3D4

/**
 * @brief VGA controller cursor port.
 */
#define VGA_CRTC_CURSOR_PORT 0x3D5

/**
 * @brief Cursor location low byte.
 */
#define VGA_CRTC_CURSOR_LOCATION_LOW 0x0F

/**
 * @brief Cursor location high byte.
 */
#define VGA_CRTC_CURSOR_LOCATION_HIGH 0x0E

/**
 * @brief Mask to be 'AND'ed with cursor location.
 */
#define VGA_CRTC_CURSOR_MASK 0xFF

/**
 * @brief Register with starting parameters for cursor.
 */
#define VGA_CRTC_CURSOR_START_REGISTER 0x0A

/**
 * @brief Bit in cursor start register controlling the display of the cursor.
 */
#define VGA_CRTC_CURSOR_DISABLE_BIT 5

/**
 * @brief Register with ending parameters for cursor.
 */
#define VGA_CRTC_CURSOR_END_REGISTER 0x0B

/**
 * @brief VGA color text mode color constants.
 */
typedef enum Vga_Color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
} Vga_Color;

/**
 * @brief Initializes terminal by setting properties to default and clearing
 * the screen.
 */
static void initialize(void);

/**
 * @brief Makes VGA color scheme.
 *
 * @param foreground Foreground color.
 * @param background Background color.
 *
 * @return VGA color scheme consisting of created foreground and
 * background color.
 */
static uint8_t make_color
    (Vga_Color foreground, Vga_Color background);

/**
 * @brief Writes an character to the terminal.
 *
 * @param c Character to be written.
 * @param x Terminal column to write to.
 * @param y Terminal row to write to.
 */
static void write_char(const char c, size_t x, size_t y);

/**
 * @brief Writes a VGA entry to the buffer. A VGA entry consists of a
 * character and it's color scheme.
 *
 * @param c Character to be written.
 * @param color VGA color scheme to write with.
 * @param x Terminal column to write to.
 * @param y Terminal row to write to.
 */
static void write_entry
    (const char c, uint8_t color, size_t x, size_t y);

/**
 * @brief Copies VGA entry from one location in the buffer to another.
 *
 * @param source_x Source column.
 * @param source_y Source row.
 * @param dest_x Destination column.
 * @param dest_y Destination row.
 */
static void copy_entry
    (size_t source_x, size_t source_y,
     size_t dest_x, size_t dest_y);

/**
 * @brief Scrolls up the terminal one row.
 */
static void scroll_up(void);

/**
 * @brief Handles character for writing to the screen or taking the
 * appropriate action for that character.
 *
 * @param c Character to be handled.
 */
static void handle_char(const char c);

/**
 * @brief Writes string to buffer, handling characters appropriately.
 *
 * @param str String to be written.
 */
static void write_string(const char* str);

/**
 * @brief Clears the terminal buffer to initial state.
 */
static void clear_screen(void);

/**
 * @brief Moves the cursor to the given location on the screen.
 *
 * @param x Column to move cursor to.
 * @param y Row to move cursor to.
 */
static void move_cursor(size_t x, size_t y);

/**
 * @brief Checks whether the cursor is visible.
 *
 * @return True if cursor is visible, false is cursor is hidden.
 */
static bool check_cursor_visible(void);

/**
 * @brief Hides the cursor, regardless of current visibility status.
 */
static void hide_cursor(void);

/**
 * @brief Shows the cursor, regardless of current status.
 */
static void show_cursor(void);

/**
 * @brief Current row for writing.
 */
static size_t row;

/**
 * @brief Current column for writing.
 */
static size_t column;

/**
 * @brief Current color for writing.
 */
static uint8_t color;

/**
 * @brief Pointer to VGA color text mode buffer location.
 */
static uint16_t* buffer = (uint16_t*) VGA_COLOR_TEXT_MODE_BUFFER;

Terminal_Driver vga_color_text_mode_get_driver(void)
{
    Terminal_Driver driver;

    driver.terminal_width = VGA_WIDTH;
    driver.terminal_height = VGA_HEIGHT;

    driver.terminal_initialize = initialize;
    driver.terminal_write_char = write_char;
    driver.terminal_copy_entry = copy_entry;
    driver.terminal_clear_screen = clear_screen;
    driver.terminal_check_cursor_visible = check_cursor_visible;
    driver.terminal_move_cursor = move_cursor;
    driver.terminal_show_cursor = show_cursor;
    driver.terminal_hide_cursor = hide_cursor;

    return (driver);
}

static void initialize(void)
{
    column = 0;
    row = 0;
    buffer = (uint16_t*) VGA_COLOR_TEXT_MODE_BUFFER;
    color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    clear_screen();
    hide_cursor();
}

static uint8_t make_color
    (Vga_Color foreground, Vga_Color background)
{
    uint8_t color = foreground | (background << 4);

    return (color);
}

static void write_char
    (const char c, size_t x, size_t y)
{
    uint16_t entry = c | (color << 8);
    size_t i = y * VGA_WIDTH + x;
    buffer[i] = entry;
}

static void write_entry
    (const char c, uint8_t color, size_t x, size_t y)
{
    uint16_t entry = c | (color << 8);
    size_t i = y * VGA_WIDTH + x;
    buffer[i] = entry;
}

static void copy_entry
    (size_t source_x, size_t source_y, size_t dest_x, size_t dest_y)
{
    size_t source = source_y * VGA_WIDTH + source_x;
    size_t dest = dest_y * VGA_WIDTH + dest_x;
    buffer[dest] = buffer[source];
}

static void scroll_up(void)
{
    column = 0;
    for (size_t y = 0; y < (VGA_HEIGHT - 1); y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            copy_entry(x, y + 1, x, y);
        }
    }
    for ( size_t x = 0; x < VGA_WIDTH; x++ )
    {
        write_entry(' ', color, x, VGA_HEIGHT - 1);
    }
}

static void handle_char(const char c)
{
    switch (c)
    {
    /* Handles newline character. */
    case '\n':
        if (row < (VGA_HEIGHT - 1))
        {
            row++;
            column = 0;
        }
        else
        {
            scroll_up();
        }
        break;
    /* Interprets as printable character. */
    default:
        write_entry
            (c,
            color,
            column,
            row);
        if (column < (VGA_WIDTH - 1))
        {
            column++;
        }
        else
        {
            if (row < (VGA_HEIGHT - 1))
            {
                row++;
                column = 0;
            }
            else
            {
                scroll_up();
            }
        }
        break;
    }
}

static void write_string(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        handle_char(str[i]);
    }
}

static void clear_screen (void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            write_entry(' ', color, x, y);
        }
    }
    column = 0;
    row = 0;
}

static void move_cursor(size_t x, size_t y)
{
    uint16_t i = (y * VGA_WIDTH) + x;
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_LOCATION_LOW);
    port_outb
        (VGA_CRTC_CURSOR_PORT,
        (uint8_t) (i & VGA_CRTC_CURSOR_MASK));
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_LOCATION_HIGH);
    port_outb
        (VGA_CRTC_CURSOR_PORT,
        (uint8_t) ((i >> 8) & VGA_CRTC_CURSOR_MASK));
}

static bool check_cursor_visible(void)
{
    bool retval;
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_START_REGISTER);
    retval =
        (
            !((port_inb(VGA_CRTC_CURSOR_PORT))
            &&
            (1 << VGA_CRTC_CURSOR_DISABLE_BIT))
        );
    return (retval);
}

static void hide_cursor(void)
{
    uint8_t value;
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_START_REGISTER);
    value = port_inb(VGA_CRTC_CURSOR_PORT);
    value |= (1 << VGA_CRTC_CURSOR_DISABLE_BIT);
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_START_REGISTER);
    port_outb
        (VGA_CRTC_CURSOR_PORT,
        value);
}

static void show_cursor(void)
{
    uint8_t value;
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_START_REGISTER);
    value = port_inb(VGA_CRTC_CURSOR_PORT);
    value &= ~(1 << VGA_CRTC_CURSOR_DISABLE_BIT);
    port_outb
        (VGA_CRTC_PORT,
        VGA_CRTC_CURSOR_START_REGISTER);
    port_outb
        (VGA_CRTC_CURSOR_PORT,
        value);
}
