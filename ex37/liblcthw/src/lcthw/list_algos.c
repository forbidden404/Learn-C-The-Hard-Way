#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

void swap(ListNode *lhs, ListNode *rhs)
{
    void *tmp = lhs->value;
    lhs->value = rhs->value;
    rhs->value = tmp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
    if (List_count(list) <= 1) return 0;

    int swapped = 0;
    do {
        swapped = 0;
        LIST_FOREACH(list, first, next, cur) {
            if (cur->prev) {
                if (cmp(cur->prev->value, cur->value) > 0) {
                    swap(cur->prev, cur);
                    swapped = 1;
                }
            }
        }
    } while (swapped);

    return 0;
}

List *merge(List *lhs, List *rhs, List_compare cmp)
{
    List *result = List_create();
    void *val = NULL;

    while (List_count(lhs) > 0 || List_count(rhs) > 0) {
        if (List_count(lhs) > 0 && List_count(rhs) > 0) {
            if (cmp(List_first(lhs), List_first(rhs)) <= 0) {
                val = List_shift(lhs);
            } else {
                val = List_shift(rhs);
            }

            List_push(result, val);
        } else if (List_count(lhs) > 0) {
            val = List_shift(lhs);
            List_push(result, val);
        } else if (List_count(rhs) > 0) {
            val = List_shift(rhs);
            List_push(result, val);
        }
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if (List_count(list) <= 1) return list;

    List *left = List_create();
    List *right = List_create();
    int middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur) {
        if (middle > 0) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }

        middle--;
    }

    List *sorted_left = List_merge_sort(left, cmp);
    List *sorted_right = List_merge_sort(right, cmp);

    if (sorted_left != left) List_destroy(left);
    if (sorted_right != right) List_destroy(right);

    return merge(sorted_left, sorted_right, cmp);
}
