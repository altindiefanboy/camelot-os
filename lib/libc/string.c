/*
 * string.c
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

#include <string.h>

void* memchr(const void* cs, int c, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        if ((((uint8_t*) cs)[i]) == ((uint8_t) c))
        {
            return ((void*) &(((uint8_t*) cs)[i]));
        }
    }
    return (NULL);
}

int memcmp(const void* cs, const void* ct, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        if ((((unsigned char*) cs)[i]) > (((unsigned char*) ct)[i]))
        {
            return (1);
        }

        if ((((unsigned char*) cs)[i]) < (((unsigned char*) ct)[i]))
        {
            return (-1);
        }
    }

    /* Objects are identical, at least up until n. */
    return (0);
}

void* memcpy(void* s, const void* ct, size_t n)
{
    /* If locations are the same, there is no reason to copy. */
    if (s == ct)
    {
        return (s);
    }

    for (size_t i = 0; i < n; ++i)
    {
        ((uint8_t*) s)[i] = ((uint8_t*) ct)[i];
    }
    return (s);
}

void* memmove(void* s, const void* ct, size_t n)
{
    /* If locations are the same, there is no reason to copy anything. */
    if (s == ct)
    {
        return (s);
    }

    /* If s is lower in memory than ct, then copy memory backwards. */
    if (s < ct)
    {
        for (size_t i = 0; i < n; ++i)
        {
            ((uint8_t*) s)[n - 1 - i] = ((uint8_t*) s)[n - 1 - i];
        }
        return (s);
    }
    /* If s is higher in memory than ct, then copy memory forwards. */
    else
    {
        for (size_t i = 0; i < n; ++i)
        {
            ((uint8_t*) s)[i] = ((uint8_t*) ct)[i];
        }
        return (s);
    }
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* s1 = (unsigned char*) s;
    unsigned char c1 = (unsigned char) c;
    for (size_t i = 0; i < n; ++i)
    {
        s1[i] = c1;
    }
    return (s1);
}

char* strcat(char* s, const char* ct)
{
    size_t s_length = strlen(s);
    size_t ct_length = strlen(ct);

    for (size_t i = 0; ct[i] != '\0'; ++i)
    {
        s[s_length + i] = ct[i];
    }
    s[s_length + ct_length] = '\0';

    return (s);
}

char* strchr(const char* cs, int c)
{
    for (size_t i = 0; cs[i] != '\0'; ++i)
    {
        if (cs[i] == ((char) c))
        {
            return (&cs[i]);
        }
    }
    return (NULL);
}

int strcmp(const char* cs, const char* ct)
{
    for (size_t i = 0; (cs[i] != '\0') && (ct[i] != '\0'); ++i)
    {
        if (((unsigned char) cs[i]) > ((unsigned char) ct[i]))
        {
            return (1);
        }

        if (((unsigned char) cs[i]) < ((unsigned char) ct[i]))
        {
            return (-1);
        }
    }

    if (strlen(cs) > strlen(ct))
    {
        return (1);
    }

    if (strlen(cs) < strlen(ct))
    {
        return (-1);
    }

    return (0);
}

char* strcpy(char* s, const char* ct)
{
    size_t ct_length = strlen(ct);

    for (size_t i = 0; i < ct_length; ++i)
    {
        s[i] = ct[i];
    }
    s[ct_length] = '\0';

    return (s);
}

size_t strcspn(const char* cs, const char* ct)
{
    size_t length = 0;

    for (size_t i = 0; cs[i] != '\0'; ++i)
    {
        if (strchr(ct, cs[i]) == NULL)
        {
            ++length;
        }
        else
        {
            return (length);
        }
    }

    return (length);
}

char* strerror(int n)
{

}

size_t strlen(const char* cs)
{
    size_t ret = 0;
    for (size_t i = 0; cs[i] != '\0'; ++i)
    {
        ++ret;
    }
    return (ret);
}

char* strncat(char* s, const char* ct, size_t n)
{
    size_t s_length = strlen(s);
    size_t ct_length = strlen(ct);

    for (size_t i = 0; (i < ct_length) && (i < n); ++i)
    {
        s[s_length + i] = ct[i];
    }

    for (size_t i = ct_length; i < n; ++i)
    {
        s[s_length + i] = '\0';
    }

    return (s);
}

char* strpbrk(const char* cs, const char* ct)
{
    char* str_end = &cs[strlen(cs)];
    char* ret = str_end;

    for (size_t i = 0; cs[i] != '\0'; ++i)
    {
        for (size_t i1 = 0; ct[i1] != '\0'; ++i1)
        {
            char* first = strchr(cs, ct[i1]);

            /* If first is closer to the beginning that any others
             * so far. */
            if ((first < ret) && (first != NULL))
            {
                ret = first;
            }
        }
    }

    if (ret != str_end)
    {
        return (ret);
    }
    else
    {
        return (NULL);
    }
}

char* strrchr(const char* cs, int c)
{
    for (size_t i = strlen(cs); i >= 0; --i)
    {
        if (cs[i] == ((char) c))
        {
            return (&cs[i]);
        }
    }
    return (NULL);
}

/**
 * @brief Reverses a string.
 *
 * @param s String to place reversed string in.
 * @param ct String to reverse.
 *
 * @note This will work even if the strings overlap or are the same string.
 */
char* strrev(char* s, const char* ct)
{
    char tmp;
    size_t length = strlen(ct);
    size_t end = (length - 1);

    for (size_t i = 0; i < length / 2; ++i)
    {
        tmp = ct[i];
        s[i] = ct[end - i];
        s[end - i] = tmp;
    }

    s[length] = '\0';

    return (s);
}

size_t strspn(const char* cs, const char* ct)
{
    size_t length = 0;

    for (size_t i = 0; cs[i] != '\0'; ++i)
    {
        if (strchr(ct, cs[i]) != NULL)
        {
            ++length;
        }
        else
        {
            return (length);
        }
    }

    return (length);
}

char* strstr(const char* cs, const char* ct)
{
	char* ret;

	ret = strchr(cs, ct[0]);
	while (ret != NULL)
	{
		for (size_t i = 0; (ret[i] == ct[i]) || (ct[i] == '\0'); ++i)
		{
			if (ct[i] == '\0')
			{
				return (ret);
			}
		}

		ret = strchr(ret, ct[0]);
	}

	return (ret);
}

char* strtok(char* s, const char* ct)
{
    /* Points to next token. */
    static char* _strtok_next = NULL;

    /* If continuing from previous token. */
    if (s == NULL)
    {
        char* current_token = _strtok_next;
        char* token_end = strpbrk(current_token, ct);

        /* If there are no more tokens. */
        if (token_end == NULL)
        {
            _strtok_next = NULL;
            return (current_token);
        }
        else
        {
            *token_end = '\0';
            _strtok_next = token_end + 1;

            return (current_token);
        }
    }
    /* If tokenizing new string. */
    else
    {
        char* token_end = strpbrk(s, ct);

        /* If there are no more tokens. */
        if (token_end == NULL)
        {
            _strtok_next = NULL;
            return (s);
        }
        /* If there are more tokens. */
        else
        {
            *token_end = '\0';
            _strtok_next = token_end + 1;
            return (s);
        }
    }
}
