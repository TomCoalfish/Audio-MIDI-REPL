#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linenoise.h"

#define UTF8

#ifdef UTF8
#include "encodings/utf8.h"
#endif

void completion(const char *buf, linenoiseCompletions *lc) {
    if (buf[0] == 'h') {
#ifdef UTF8
        linenoiseAddCompletion(lc,"hello こんにちは");
        linenoiseAddCompletion(lc,"hello こんにちは there");
        linenoiseAddCompletion(lc,"hello こんにちは 👨‍💻");
#else
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
#endif
    }
}

char *hints(const char *buf, int *color, int *bold) {
    if (!strcasecmp(buf,"hello")) {
        *color = 35;
        *bold = 0;
        return " World";
    }
    if (!strcasecmp(buf,"こんにちは")) {
        *color = 35;
        *bold = 0;
        return " 世界";
    }
    return NULL;
}

int main(int argc, char **argv) {
    char *line;
    char *prgname = argv[0];

		linenoiseApp *app = linenoiseAppNew(STDIN_FILENO, STDOUT_FILENO);

		if(!app)
		{
			exit(1);
		}

    /* Parse options, with --multiline we enable multi line editing. */
    while(argc > 1) {
        argc--;
        argv++;
        if (!strcmp(*argv,"--multiline")) {
            linenoiseSetMultiLine(app, 1);
            printf("Multi-line mode enabled.\n");
        } else if (!strcmp(*argv,"--keycodes")) {
            linenoisePrintKeyCodes(app);
            exit(0);
        } else {
            fprintf(stderr, "Usage: %s [--multiline] [--keycodes]\n", prgname);
            exit(1);
        }
    }

#ifdef UTF8
    linenoiseSetEncodingFunctions(app,
        linenoiseUtf8PrevCharLen,
        linenoiseUtf8NextCharLen,
        linenoiseUtf8ReadCode);
#endif

    /* Set the completion callback. This will be called every time the
     * user uses the <tab> key. */
    linenoiseSetCompletionCallback(app, completion);
    linenoiseSetHintsCallback(app, hints);

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad(app, "history.txt"); /* Load the history at startup */

    /* Now this is the main loop of the typical linenoise-based application.
     * The call to linenoise() will block as long as the user types something
     * and presses enter.
     *
     * The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */
#ifdef UTF8
    while((line = linenoise(app, "\033[32mこんにちは\x1b[0m> ")) != NULL) {
#else
    while((line = linenoise(app, "\033[32mhello\x1b[0m> ")) != NULL) {
#endif
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            printf("echo: '%s'\n", line);
            linenoiseHistoryAdd(app, line); /* Add to the history. */
            linenoiseHistorySave(app, "history.txt"); /* Save the history on disk. */
        } else if (!strncmp(line,"/historylen",11)) {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(app, len);
        } else if (!strncmp(line, "/mask", 5)) {
            linenoiseMaskModeEnable(app);
        } else if (!strncmp(line, "/unmask", 7)) {
            linenoiseMaskModeDisable(app);
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }

		linenoiseAppFree(app);

    return 0;
}
