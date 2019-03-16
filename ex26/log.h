#ifndef _log_h
#define _log_h

#include "options.h"

char **read_log_files_list(int *length);
void read_logs(char **list, int list_length, options_t *options);

#endif // _log_h
