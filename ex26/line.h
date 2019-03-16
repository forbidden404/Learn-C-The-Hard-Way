#ifndef _line_h
#define _line_h

#include <stdio.h>
#include <stdlib.h>

long get_input_frag(
        /* @ out @ */char *buf, long len,
        int term_char,
        int (* cb_getc) (void *ctx), void *cb_getc_ctx);

static int xfgetc(void *ctx)
{
    return fgetc((FILE *) ctx);
}

long fgetstr(char *buf, int n, FILE *fp);

#define getstr(buff, n) fgetstr((buff), (n), stdin)

size_t fgetstralloc(char **p, FILE *fp);

#define getstralloc(p) fgetstralloc((p), stdin)


#endif // _line_h
