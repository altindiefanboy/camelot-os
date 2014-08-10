/**
 * @file assert.h
 *
 * @author Seth Nils
 * @date 2014/6/5
 *
 * @note Macro "assert" cannot be properly implimented until "stderr" and
 * "abort" are implemented.
 */

#error File "assert.h" is a stub.

#ifndef _ASSERT_H_INCLUDED
#define _ASSERT_H_INCLUDED

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NDEBUG
#define assert(x) (stderr << "#x, file __FILE__, line __LINE__"; abort())
#else
#define assert(expression) (void (expression))
#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif

#endif /* _ASSERT_H_INCLUDED */
