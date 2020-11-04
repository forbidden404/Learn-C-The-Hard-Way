#include "minunit.h"
#include <lcthw/darray_algos.h>
#include <time.h>

int testcmp(char **a, char **b)
{
    return strcmp(*a, *b);
}

DArray *create_words()
{
    DArray *result = DArray_create(0, 5);
    char *words[] = { "asdfasfd", "werwar", "13234", "asdfasfd", "oioj" };
    int i = 0;

    for (i = 0; i < 5; i++) {
        DArray_push(result, words[i]);
    }

    return result;
}

int is_sorted(DArray *array)
{
    int i = 0;

    for (i = 0; i < DArray_count(array) - 1; i++) {
        if (strcmp(DArray_get(array, i), DArray_get(array, i + 1)) > 0) {
            return 0;
        }
    }

    return 1;
}

char *run_sort_test(int (*func) (DArray *, DArray_compare), const char *name)
{
    DArray *words = create_words();

    mu_assert(!is_sorted(words), "Words should start not sorted.");

    debug("--- Testing %s sorting algorithm", name);
    clock_t t;
    t = clock();
    int rc = func(words, (DArray_compare) testcmp);
    t = clock() - t;
    mu_assert(rc == 0, "sort failed.");
    mu_assert(is_sorted(words), "didn't sort it.");

    DArray_destroy(words);

    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    char *s = NULL;
#ifdef DASORT
    s = "-DDASORT is defined";
#else
    s = "-DDASORT is not defined";
#endif
    printf("The %s sorting algorithm when %s took %f seconds to run.\n", name, s, time_taken);

    return NULL;
}

char *test_qsort()
{
    return run_sort_test(DArray_qsort, "qsort");
}

char *test_heapsort()
{
    return run_sort_test(DArray_heapsort, "heapsort");
}

char *test_mergesort()
{
    return run_sort_test(DArray_mergesort, "mergesort");
}

char *test_sort_add()
{
    DArray *words = create_words();

    mu_assert(!is_sorted(words), "Words should start not sorted.");

    char *data = "98765";
    int rc = DArray_sort_add(words, data, (DArray_compare) testcmp);
    mu_assert(rc == 0, "sort_add failed.");
    mu_assert(is_sorted(words), "didn't sort it.");
    char *result = words->contents[1];
    mu_assert(testcmp(&result, &data) == 0, "\"98765\" should be the second element of the array.");

    DArray_destroy(words);

    return NULL;
}

char *test_find()
{
    DArray *words = create_words();

    mu_assert(!is_sorted(words), "Words should start not sorted.");

    int rc = DArray_qsort(words, (DArray_compare) testcmp);
    mu_assert(rc == 0, "sort failed.");
    mu_assert(is_sorted(words), "didn't sort it.");

    char *data = "13234";
    int index = DArray_find(words, data, (DArray_compare) testcmp);
    mu_assert(index != -1, "Couldn't find \"13234\"");
    char *result = DArray_get(words, index);
    mu_assert(testcmp(&result, &data) == 0, "Wrong index.");

    DArray_destroy(words);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);
    mu_run_test(test_sort_add);
    mu_run_test(test_find);

    return NULL;
}

RUN_TESTS(all_tests);
