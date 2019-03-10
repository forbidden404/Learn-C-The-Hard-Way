#include <stdio.h>

char lowercase(char letter)
{
    if (letter >= 'A' && letter <= 'Z') {
        return letter + ('a' - 'A');
    } else {
        return letter;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("ERROR: You need at least one argument.\n");
        // this is how you abort a program
        return 1;
    }

    int i = 0;
    int count = 1;
    for (count = 1; count < argc; count++) 
    for (i = 0; argv[count][i] != '\0'; i++) {
        char letter = lowercase(argv[count][i]);

        switch (letter) {
            case 'a':
                printf("%d: 'A'\n", i);
                break;

            case 'e':
                printf("%d: 'E'\n", i);
                break;

            case 'i':
                printf("%d: 'I'\n", i);
                break;

            case 'o':
                printf("%d: 'O'\n", i);
                break;

            case 'u':
                printf("%d: 'U'\n", i);
                break;
           
            case 'y':
                if (i > 2) {
                    // it's only sometimes Y
                    printf("%d: 'Y'\n", i);
                }
                break;

            default:
                printf("%d: %c is not a vowel\n", i, letter);
        }
    }

    return 0;
}
