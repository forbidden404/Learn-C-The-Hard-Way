#include "dbg.h"
#include "options.h"
#include "log.h"

int main(int argc, char *argv[])
{
    options_t *options = process_args(argc, argv);
    check(options, "Could not create options");

    int list_length = 0;
    char **list = read_log_files_list(&list_length);
    log_info("List length: %d", list_length);

    read_logs(list, list_length, options);

    free_options(options);

    return 0;

error:
    if (options) free_options(options);

    return -1;
}
