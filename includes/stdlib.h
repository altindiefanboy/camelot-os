/*
 * stdlib.h
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

#ifndef _STDLIB_H_INCLUDED
#define _STDLIB_H_INCLUDED

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Converts string to double.
 *
 * @param s String to convert.
 *
 * @return String s converted to type double.
 */
double atof(const char* s);

/**
 * @brief Converts string to int.
 *
 * @param s String to convert.
 *
 * @return String s converted to type int.
 */
int atoi(const char* s);

/**
 * @brief Converts string to long.
 *
 * @param s String to convert.
 *
 * @return String s converted to type long.
 */
long atol(const char* s);

/**
 * @brief Converts the prefix of a string to double, ignoring leading white
 * space; it stores a pointer to any unconverted suffix in a pointer unless
 * the pointer provided is NULL.
 *
 * @param s String containing prefix to convert.
 * @param endp Pointer in which to store any unconverted suffix from s.
 *
 * @return Prefix of s converted to double. If the answer would overflow,
 * HUGE_VAL is returned with the proper sign; if the answer would underflow,
 * zero is returned. In either error case, errno is set to ERANGE.
 */
double strtod(const char* s, char** endp);

/**
 * @brief Converts the prefix a string to long, ignoring leading white space;
 * it stores a pointer to any unconverted suffix in a pointer unless the
 * pointer provided is NULL. Conversion is done assuming that the input is
 * of the given base.
 *
 * @param s String containing prefix to convert.
 * @param endp Pointer in which to store any unconverted suffix from s.
 * @param base Base in which to convert. If base is between 2 and 36,
 * conversion is done assuming that the input is written in that base. If base
 * is 0, the base will be assumed to be either 8, 10, or 16; leading 0
 * implies octal, and leading 0x or 0X implies hexadecimal. Letters in either
 * case represent digits from 10 to base - 1; a leading 0x or 0X is permitted
 * in base 16.
 *
 * @return Prefix of s converted to long according to base. If the answer
 * would overflow, LONG_MAX or LONG_MIN is returned, depending on the sign
 * of the result, and errno is set to ERANGE.
 */
long strtol(const char* s, char** endp, int base);

/**
 * @brief Converts the prefix a string to unsigned long, ignoring leading
 * white space; it stores a pointer to any unconverted suffix in a pointer
 * unless the pointer provided is NULL. Conversion is done assuming that the
 * input is of the given base.
 *
 * @param s String containing prefix to convert.
 * @param endp Pointer in which to store any unconverted suffix from s.
 * @param base Base in which to convert. If base is between 2 and 36,
 * conversion is done assuming that the input is written in that base. If base
 * is 0, the base will be assumed to be either 8, 10, or 16; leading 0
 * implies octal, and leading 0x or 0X implies hexadecimal. Letters in either
 * case represent digits from 10 to base - 1; a leading 0x or 0X is permitted
 * in base 16.
 *
 * @return Prefix of s converted to long according to base. If the answer
 * would overflow, ULONG_MAX is returned, and errno is set to ERANGE.
 */
unsigned long strtoul(const char* s, char** endp, int base);

/**
 * @brief Returns a pseudo-random integer in the range 0 to RAND_MAX, which
 * is at least 32767.
 *
 * @return Pseudo-random integer generated.
 */
int rand(void);

/**
 * @brief Uses seed as the seed for a new sequence of pseudo-random numbers.
 * The initial seed is 1.
 */
void srand(unsigned int seed);

/**
 * @brief Allocates memory for an array of objects of a given size,
 * initialized to zero bytes.
 *
 * @param n Number of objects.
 * @param size Size of memory to allocate for each object, in bytes.
 *
 * @return Pointer to beginning of memory block allocated. Returns NULL if
 * request cannot be satisfied.
 */
void* calloc(size_t n, size_t size);

/**
 * @brief Allocates memory for an object of a given size.
 *
 * @param size Size of memory to allocate, in bytes.
 *
 * @return Pointer to beginning of memory block allocated. Returns NULL if
 * request cannot be satisfied.
 */
void* malloc(size_t size);

/**
 * @brief Changes size of object pointed to by a specified object.
 *
 * @param ptr Pointer to resize.
 * @param size New size of object, in bytes.
 *
 * @return Pointer to beginning of new memory block. Returns NULL if
 * request cannot be satisfied, in which case the pointer is unchanged.
 */
void* realloc(void* ptr, size_t size);

/**
 * @brief Deallocates the space pointed to by pointer; it does nothing if
 * pointer is NULL.
 *
 * @param ptr Pointer to memory to deallocate. It must be a pointer to a space
 * previously alllocated by calloc, malloc, or realloc.
 */
void free(void* ptr);

/**
 * @brief Searches array for an item that matches the key specified using a
 * specified function.
 *
 * @param key Key to search for.
 * @param base Pointer to first element of array. Items in the array must be
 * in ascending order.
 * @param n Number of elements in array.
 * @param size Size of each array element.
 * @param cmp Function pointer to function that will be used to compare
 * elements. Must return negative if its first argument (the search key) is
 * less than its second (a table entry), zero if equal, and positive if
 * greater.
 *
 * @return Pointer to a matching item, or NULL if none exists.
 */
void* bsearch(
    const void* key,
    const void* base,
    size_t n,
    size_t size,
    int (*cmp)(const void* keyval, const void* datum));

/**
 * @brief Sorts array elements into ascending order.
 *
 * @param base Pointer to first element of array.
 * @param n Number of elements in array.
 * @param size Size of each array element.
 * @param cmp Function pointer to function that will be used to compare
 * elements. Must return negative if its first argument (the search key) is
 * less than its second (a table entry), zero if equal, and positive if
 * greater.
 */
void qsort
    (void* base, size_t n, size_t size,
     int (*cmp)(const void*, const void*));

/**
 * @brief Returns absolute value of an integer.
 *
 * @param n Value to find absolute value of.
 *
 * @return Absolute value of value n.
 */
int abs(int n);

/**
 * @brief Returns absolute value of a long integer.
 *
 * @param n Value to find absolute value of.
 *
 * @return Absolute value of value n.
 */
long labs(long n);

/**
 * @brief Returns absolute value of a double.
 *
 * @param n Value to find absolute value of.
 *
 * @return Absolute value of value n.
 */
double fabs(double n);

/**
 * @brief Stores quotient and remainder of an integer divison operation.
 */
typedef struct div_t
{
    int quot;
    int rem;
} div_t;

/**
 * @brief Stores quotient and remainder of a long integer divison operation.
 */
typedef struct ldiv_t
{
    long quot;
    long rem;
} ldiv_t;

/**
 * @brief Computes the quotient and remainder of an integer division
 * operation.
 *
 * @param num Numerator.
 * @param denom Denominator.
 *
 * @return Struct div_t storing the quotient and remainder of the division
 * operation.
 */
div_t div(int num, int denom);

/**
 * @brief Computes the quotient and remainder of a long integer divison
 * operation.
 *
 * @param num Numerator.
 * @param denom Denominator.
 *
 * @return Struct ldiv_t storing the quotient and remainder of the division
 * operation.
 */
ldiv_t ldiv(long num, long denom);

/* The following are not found in the standard library, bu have still been
 * implemented because they are commonly supported and used and/or fit the
 * purpose of this header.
 */

/**
 * @brief Converts value to string.
 *
 * @param val Value to convert.
 * @param str String to store converted value in.
 * @param base Base the number is represented in.
 *
 * @return Null-terminated string containing value converted to a string.
 *
 * @note If base is 10 and value is negative, the resulting string will
 * be preceded by a negative sign ('-'). With any other base, value is
 * always considered to be unsigned.
 *
 * @note String should be an array long enough to contain any possible value.
 * (ex. at least ((sizeof(int) * 8 + 1) or 33 bytes on 32-bit
 * platforms)
 *
 * @note Base should be between 2 and 36, where 10 means decimal base, 16
 * means hexadecimal base, 8 means octal base, and 2 means binary base.
 */
char* itoa(int val, char* str, int base);

/**
 * @brief Converts value to string.
 *
 * @param val Value to convert.
 * @param str String to store converted value in.
 * @param base Base the number is represented in.
 *
 * @return Null-terminated string containing value converted to a string.
 *
 * @note If base is 10 and value is negative, the resulting string will
 * be preceded by a negative sign ('-'). With any other base, value is
 * always considered to be unsigned.
 *
 * @note String should be an array long enough to contain any possible value.
 * (ex. at least ((sizeof(int) * 8 + 1) or 33 bytes on 32-bit
 * platforms)
 *
 * @note Base should be between 2 and 36, where 10 means decimal base, 16
 * means hexadecimal base, 8 means octal base, and 2 means binary base.
 */
char* litoa(long val, char* str, int base);

/**
 * @brief Converts value to string.
 *
 * @param val Value to convert.
 * @param str String to store converted value in.
 * @param base Base the number is represented in.
 *
 * @return Null-terminated string containing value converted to a string.
 *
 * @note If base is 10 and value is negative, the resulting string will
 * be preceded by a negative sign ('-'). With any other base, value is
 * always considered to be unsigned.
 *
 * @note String should be an array long enough to contain any possible value.
 * (ex. at least ((sizeof(int) * 8 + 1) or 33 bytes on 32-bit
 * platforms)
 *
 * @note Base should be between 2 and 36, where 10 means decimal base, 16
 * means hexadecimal base, 8 means octal base, and 2 means binary base.
 */
char* sitoa(size_t val, char* str, int base);

/**
 * @brief Converts raw byte to string.
 *
 * @param val Byte to convert.
 * @param str String to store converted byte in.
 * @param bits Number of bits to convert. If this value is greater than 8, it
 * will be rounded down to 8.
 *
 * @return Null-terminated string containing byte converted to a string.
 *
 * @note String should be an array long enough to store (bits + 1) characters,
 * or 9 characters if bits is greater than 8.
 */
char* btoa(uint8_t val, char* str, size_t bits);

/* The following are all parts of the standard library that have not been
 * implemented because the do not apply in a freestanding environment. */
#if 0

/**
 * @brief Causes normal program termination. atexit functions are called in
 * reverse order of registration, open files are flushed, open streams are
 * closed, and control is returned to the environment. How status is returned
 * to the environment is implementation-dependent, but zero is taken as
 * successful termination. The values EXIT_SUCCESS and EXIT_FAILURE may also
 * be used.
 *
 * @param status Exit code to be returned to the environment.
 *
 * @note This function may not apply in a freestanding environment.
 */
void exit(int status);

/**
 * @brief Registers a function to be called when the program terminates
 * normally.
 *
 * @param fcn Function to register.
 *
 * @return Zero if the registration can be made; non-zero if the registration
 * cannot be made.
 *
 * @note This function may not apply in a freestanding environment.
 */
int atexit(void (*fcn)(void));

/**
 * @brief Passes a string to the environment for execution, or tests for a
 * command processor.
 *
 * @param s String to pass for execution. If s is NULL, tests for a command
 * processor.
 *
 * @return If s is NULL, returns non-zero if there is a command processor.
 * If s is not NULL, the return value is implementation-dependent.
 *
 * @note This function may not apply in a freestanding environment.
 */
int system(const char* s);

#endif /* 0 */

#endif /* _STDLIB_H_INCLUDED */
