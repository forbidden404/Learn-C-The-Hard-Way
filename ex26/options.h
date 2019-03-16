#ifndef _options_h
#define _options_h

typedef struct {
    int logic;
    char **argv;
    int argc;
} options_t;

options_t *process_args(int argc, char *argv[]);

void free_options(options_t *options);

#endif // _options_h
