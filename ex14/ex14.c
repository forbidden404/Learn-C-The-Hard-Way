#include <stdio.h>
#include <ctype.h>
#include <string.h>

// forward declarations
void print_letters(int length, char arg[]);

void print_arguments(int argc, char *argv[])
{
    int i = 0;

    for (i = 0; i < argc; i++) {
        print_letters(strlen(argv[i]), argv[i]);
    }
}

void print_letters(int length, char arg[])
{
    int i = 0;

    for (i = 0; i < length; i++) {
        char ch = arg[i];

        int can_print_it = isalpha(ch) || isblank(ch) || isnumber(ch);
        if (can_print_it) {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
