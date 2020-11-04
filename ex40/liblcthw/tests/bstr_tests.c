#include "minunit.h"
#include <lcthw/bstrlib.h>

const char *data = "data";

char *test_bfromcstr()
{
    bstring b = bfromcstr(data);

    mu_assert(b != NULL, "bfromcstr couldn't create a bstring.");
    
    int rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_blk2bstr()
{
    size_t len = strlen(data);
    bstring b = blk2bstr(data, len);

    mu_assert(b != NULL, "blk2bstr couldn't create a bstring.");

    int rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_bstrcpy()
{
    bstring b = bfromcstr(data);
    bstring c = bstrcpy(b);

    mu_assert(b != NULL, "bfromcstr couldn't create a bstring.");
    mu_assert(c != NULL, "bstrcpy couldn't create a bstring.");

    int rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    rc = bdestroy(c);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_bassign()
{
    bstring b = bfromcstr(data);
    bstring c = bfromStatic("");

    int rc = bassign(c, b);
    mu_assert(rc == BSTR_OK, "Failed to assign one bstring to another.");

    rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    rc = bdestroy(c);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_bassigncstr()
{
    bstring b = bfromStatic("");

    int rc = bassigncstr(b, data);
    mu_assert(rc == BSTR_OK, "Failed to assign a cstring to a bstring.");

    rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_bassignblk()
{
    bstring b = bfromStatic("");

    size_t len = strlen(data);
    int rc = bassignblk(b, data, len);

    mu_assert(rc == BSTR_OK, "Failed to assign a cstring to a bstring with a given length.");

    rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *test_aux_bstr()
{
    bstring b = bfromStatic("");
    bstring c = bfromcstr(data);

    int rc = bconcat(b, c);
    mu_assert(rc == BSTR_OK, "Failed to concatenate two bstrings.");

    rc = bstricmp(b, c);
    mu_assert(rc == 0, "bstrings should be equal.");

    rc = biseq(b, c);
    mu_assert(rc == 1, "bstrings should be equal.");

    rc = binstr(b, 0, c);
    mu_assert(rc == 0, "bstring b should be in bstring c starting at position 0.");

    rc = bfindreplace(b, bfromStatic("ata"), bfromStatic("ive"), 0);
    mu_assert(rc == 0, "find_replace failed.");
    mu_assert(biseq(b, bfromStatic("dive")) == 1, "bstring is not correct after find_replace.");

    rc = blength(b);
    mu_assert(rc == 4, "blength of b should be 4.");

    rc = strcmp(bdata(b), "dive");
    mu_assert(rc == 0, "strcmp failed while using bdata.");

    mu_assert(bchar(b, 0) == 'd', "First character doesn't match.");

    bstring numbers = bformat("%d%d%d%d%d%d%d%d%d%d", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    rc = biseq(numbers, bfromStatic("0123456789"));
    mu_assert(rc == 1, "string created from bformat is not right.");

    struct bstrList *list = bsplit(numbers, '5');
    mu_assert(biseq(list->entry[0], bfromStatic("01234")) == 1, "Splitted string should have first entry as 01234");
    mu_assert(biseq(list->entry[1], bfromStatic("6789")) == 1, "Splitted string should have second entry as 6789");

    rc = bdestroy(b);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    rc = bdestroy(c);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    rc = bdestroy(numbers);
    mu_assert(rc == BSTR_OK, "Failed to destroy bstring.");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();
    
    mu_run_test(test_bassign);
    mu_run_test(test_bassignblk);
    mu_run_test(test_bassigncstr);
    mu_run_test(test_bfromcstr);
    mu_run_test(test_blk2bstr);
    mu_run_test(test_bstrcpy);
    mu_run_test(test_aux_bstr);

    return NULL;
}

RUN_TESTS(all_tests);