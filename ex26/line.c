#include <stdlib.h>

#include "dbg.h"
#include "line.h"

long get_input_frag(
        /* @ out @ */char *buf, long len,
        int term_char,
        int (* cb_getc) (void *ctx), void *cb_getc_ctx) 
{
    long i;
    int c;

    if (!buf || len < 0 || !cb_getc) return -1;

    for (c = (~term_char) | 1, i = 0; i < len; i++) {
        if (c == (int) '\0' || c == term_char || EOF == (c = cb_getc(cb_getc_ctx))) {
            buf[i] = '\0';
            break;
        }
        buf[i] = (char) c;
    }

    return i;
}

long fgetstr(char *buf, int n, FILE *fp)
{
    long len;

    if (!buf) return -1;

    if (1 == n) {
        buf[0] = '\0';
        return 0;
    }

    if (n < 1 || !fp) return -1;

    len = get_input_frag(buf, n-1, (int) '\n', xfgetc, fp);
    if ((len) >= 0) buf[len] = '\0';
    return len;
}

size_t fgetstralloc(char **p, FILE *fp)
{
    size_t mlen, slen, dlen, len;
    char *s, *t;

    if (!fp) return (size_t) -1;

    if (!p) {
        char blk[64];
        for (slen = 0; ; slen += 64) {
            len = (size_t) get_input_frag(blk, 64, (int) '\n', xfgetc, fp);
            if (len != 64 || blk[len - 1] == '\n' || blk[len - 1] == '\0')
                return slen + len;
        }
    }

    mlen = 8;
    slen = 0;
    if (NULL == (s = (char *) malloc(mlen))) return (size_t) -1;

    for (;;) {
        mlen += mlen;
        if (NULL == (t = realloc(s, mlen))) break;
        len = (size_t) get_input_frag((s = t) + slen, dlen = mlen - slen - 1, (int) '\n', xfgetc, fp);

        slen += len;
        if (len < dlen || t[slen - 1] == '\n' || t[slen-1] == '\0') {
            s[slen] = '\0';
            *p = s;
            return slen;
        }
    }

    free(s);
    return (size_t) -1;
}
