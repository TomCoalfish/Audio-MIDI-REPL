/*
 * SPDX-FileCopyrightText: Hanspeter Portner <dev@open-music-kontrollers.ch>
 * SPDX-License-Identifier: Artistic-2.0
 */

#ifndef _D2TK_UTIL_H
#define _D2TK_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include <d2tk/d2tk.h>

/****f* util/d2tk_util_spawn
 * SYNOPSIS
 */
D2TK_API int
d2tk_util_spawn(char **argv);
/*
 * FUNCTION
 *  If condition is false, makes a longjump back to wrapping function start.
 * INPUTS
 *  - jit       Pointer to opaque jit structure
 *  - condition	Condition, whether to longjump or not
 ****
 */

D2TK_API int
d2tk_util_kill(int *kid);

D2TK_API int
d2tk_util_wait(int *kid);

#ifdef __cplusplus
}
#endif

#endif // _D2TK_UTIL_H
