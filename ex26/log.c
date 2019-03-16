#include <glob.h>
#include <stdlib.h>

#include "dbg.h"
#include "options.h"
#include "log.h"
#include "line.h"

#define GLOB_LIST "~/.logfind/*"

char **read_log_files_list(int *length)
{
    glob_t g;

    glob(GLOB_LIST, GLOB_TILDE, NULL, &g);

    log_info("Number of files to read: %ld", g.gl_pathc);

    *length = g.gl_pathc;
    check(*length > 0, "There are no files in ~/.logfind");

    char **list = malloc(sizeof(char *) * *length);
    check(list, "Couldn't allocate memory for list");

    int i = 0;
    for (i = 0; i < *length; i++) {
        list[i] = strdup(g.gl_pathv[i]);
        log_info("File %d: %s", i, g.gl_pathv[i]);
    }

    globfree(&g);

    return list;

error:
    globfree(&g);

    return NULL;
}

void read_logs(char **list, int list_length, options_t *options)
{
    FILE *file = NULL;

    int *argvWasFound = malloc(sizeof(int) * options->argc);

    // traverse list of files
    int i = 0;
    for (i = 0; i < list_length; i++) {
        file = fopen(list[i], "r+");
        check(file, "Couldn't open the file: %s", list[i]);

        // check words in line
        char *line = NULL;
        int length = 0;

        // keep track of found lines
        int line_number = 1;
        int size = 64;
        int index = 0;
        char **lines = malloc(sizeof(char *) * size);

        while ((length = fgetstralloc(&line, file)) > 0) {
            int j = 0;
            for (; j < options->argc; j++) {
                if (strstr(line, options->argv[j]) != NULL) {
                    if (options->logic) {
                        printf("%s:%d => %s", list[i], line_number, line);
                    } else {
                        argvWasFound[j] = 1;

                        // save formatted string
                        lines[index] = malloc(sizeof(char) * (length + strlen(list[i]) + 20));
                        sprintf(lines[index++], "%s:%d => %s", list[i], line_number, line);
                        if (index == size - 1) {
                            // realloc
                            size *= 2;
                            char **s = realloc(lines, sizeof(char *) * size);
                            lines = s;
                        }
                    }
                }
            }
            line_number++;
        }

        int tmp = 0;
        int shouldPrint = 1;
        for (; tmp < options->argc; tmp++) {
            if (argvWasFound[tmp] == 0) {
                shouldPrint = 0;
            }
            argvWasFound[tmp] = 0;
        }

        if (shouldPrint) {
            int j = 0;
            for (; j < index; j++)
                printf("%s", lines[j]);
        }

        fclose(file);
    }

error:
    if (file) fclose(file);
}
