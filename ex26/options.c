#include <stdlib.h>

#include "options.h"
#include "dbg.h"

options_t *process_args(int argc, char *argv[])
{
    options_t *options = malloc(sizeof(options_t));
    check(options, "Memory error while creating options");

    options->logic = 0;
    options->argc = 0;
    options->argv = malloc(sizeof(char *) * (argc - 1));
    check(options->argv, "Memory error while allocating argv");

    int i = 1;
    for (; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'o') {
            options->logic = 1;
        } else {
            options->argv[options->argc] = strdup(argv[i]);
            options->argc++;
        }
    }

    log_info("Options created at: %p", options);
    log_info("OR Logic selected: %s", options->logic ? "true" : "false");
    log_info("Options argument count: %d", options->argc);

    return options;

error:
    if (options) free_options(options);
    return NULL;
}

void free_options(options_t *options)
{
    if (options->argv) {
        int i = 0;
        for (; i < options->argc; i++) {
            free(options->argv[i]);
        }
    }

    free(options);
}
