/* linenoise.h -- VERSION 1.0
 *
 * Guerrilla line editing library against the idea that a line editing lib
 * needs to be 20,000 lines of C code.
 *
 * See linenoise.c for more information.
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2014, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2010-2013, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LINENOISE_H
#define __LINENOISE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _linenoiseApp linenoiseApp;

linenoiseApp *linenoiseAppNew(int fd_in, int fd_out);
void linenoiseAppFree(linenoiseApp *app);

typedef struct linenoiseCompletions {
  size_t len;
  char **cvec;
} linenoiseCompletions;

typedef void(linenoiseCompletionCallback)(const char *, linenoiseCompletions *);
typedef char*(linenoiseHintsCallback)(const char *, int *color, int *bold);
typedef void(linenoiseFreeHintsCallback)(void *);
void linenoiseSetCompletionCallback(linenoiseApp *app, linenoiseCompletionCallback *);
void linenoiseSetHintsCallback(linenoiseApp *app, linenoiseHintsCallback *);
void linenoiseSetFreeHintsCallback(linenoiseApp *app, linenoiseFreeHintsCallback *);
void linenoiseAddCompletion(linenoiseCompletions *, const char *);

char *linenoise(linenoiseApp *app, const char *prompt);
void linenoiseFree(linenoiseApp *app, void *ptr);
int linenoiseHistoryAdd(linenoiseApp *app, const char *line);
int linenoiseHistorySetMaxLen(linenoiseApp *app, int len);
int linenoiseHistorySave(linenoiseApp *app, const char *filename);
int linenoiseHistoryLoad(linenoiseApp *app, const char *filename);
void linenoiseClearScreen(linenoiseApp *app);
void linenoiseSetMultiLine(linenoiseApp *app, int ml);
void linenoisePrintKeyCodes(linenoiseApp *app);
void linenoiseMaskModeEnable(linenoiseApp *app);
void linenoiseMaskModeDisable(linenoiseApp *app);
void linenoiseSetFill(linenoiseApp *app, const char *fill);

typedef size_t (linenoisePrevCharLen)(const char *buf, size_t buf_len, size_t pos, size_t *col_len);
typedef size_t (linenoiseNextCharLen)(const char *buf, size_t buf_len, size_t pos, size_t *col_len);
typedef size_t (linenoiseReadCode)(int fd, char *buf, size_t buf_len, int* c);

void linenoiseSetEncodingFunctions(linenoiseApp *app,
    linenoisePrevCharLen *prevCharLenFunc,
    linenoiseNextCharLen *nextCharLenFunc,
    linenoiseReadCode *readCodeFunc);

#ifdef __cplusplus
}
#endif

#endif /* __LINENOISE_H */
