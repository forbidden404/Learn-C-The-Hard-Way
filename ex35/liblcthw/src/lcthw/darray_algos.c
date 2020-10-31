#include <lcthw/darray_algos.h>
#include <stdlib.h>

void DArray_swap(void *a, void *b, size_t len)
{
    unsigned char *p = a, *q = b, tmp;
    size_t i = 0;
    for (i = 0; i != len; i++) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

int partition_qsort(DArray *array, DArray_compare cmp, int left, int right)
{
    // Median of three to choose pivot
    int middle = (left + right) / 2;
    if (cmp(&array->contents[middle], &array->contents[left]) < 0) {
        DArray_swap(&array->contents[middle], &array->contents[left], sizeof(void *));
    }
    if (cmp(&array->contents[right], &array->contents[left]) < 0) {
        DArray_swap(&array->contents[left], &array->contents[right], sizeof(void *));
    }
    if (cmp(&array->contents[middle], &array->contents[right]) < 0) {
        DArray_swap(&array->contents[middle], &array->contents[right], sizeof(void *));
    }

    void *pivot = DArray_get(array, right);
    int i = left - 1;
    int j = 0;
    for (j = left; j < right; j++) {
        if (cmp(&array->contents[j], &pivot) <= 0) {
            i++;
            DArray_swap(&array->contents[i], &array->contents[j], sizeof(void *));
        }
    }
    DArray_swap(&array->contents[i+1], &array->contents[right], sizeof(void *));
    return i + 1;
}

void quicksort(DArray *array, DArray_compare cmp, int left, int right)
{
    if (left < right) {
        int q = partition_qsort(array, cmp, left, right);
        quicksort(array, cmp, left, q - 1);
        quicksort(array, cmp, q + 1, right);
    }
}

int DArray_qsort(DArray *array, DArray_compare cmp)
{
#ifdef DASORT
    quicksort(array, cmp, 0, DArray_count(array) - 1);
#else
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
#endif
    return 0;
}

void heapify(DArray *array, DArray_compare cmp, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && cmp(&array->contents[l], &array->contents[largest]) > 0) {
        largest = l;
    }

    if (r < n && cmp(&array->contents[r], &array->contents[largest]) > 0) {
        largest = r;
    }

    if (largest != i) {
        DArray_swap(&array->contents[i], &array->contents[largest], sizeof(void *));
        heapify(array, cmp, n, largest);
    }
}

int DArray_heapsort(DArray *array, DArray_compare cmp)
{
#ifdef DASORT
    int i = 0;
    int n = DArray_count(array);
    for (i = n / 2 - 1; i >= 0; i--) {
        heapify(array, cmp, n, i);
    }
    for (i = n - 1; i > 0; i--) {
        DArray_swap(&array->contents[0], &array->contents[i], sizeof(void *));
        heapify(array, cmp, i, 0);
    }
    return 0;
#else
    return heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
#endif
}

void DArray_merge(DArray *array, int p, int q, int r, DArray_compare cmp)
{
    int n1 = q - p + 1;
    int n2 = r - q;

    DArray *left = DArray_create(0, n1);
    DArray *right = DArray_create(0, n2);

    int i = 0;
    for (i = 0; i < n1; i++) {
        DArray_set(left, i, DArray_get(array, p + i));
    }

    for (i = 0; i < n2; i++) {
        DArray_set(right, i, DArray_get(array, q + i + 1));
    }

    int j = 0;
    int k = p;
    while (i < n1 && j < n2) {
        if (cmp(&left->contents[i], &right->contents[j]) < 0) {
            DArray_set(array, k, DArray_get(left, i));
            i++;
            k++;
        } else {
            DArray_set(array, k, DArray_get(right, j));
            j++;
            k++;
        }
    }

    while (i < n1) {
        DArray_set(array, k, DArray_get(left, i));
        i++;
        k++;
    }

    while (j < n2) {
        DArray_set(array, k, DArray_get(right, j));
        j++;
        k++;
    }
}

void DArray_merge_sort(DArray *array, int p, int r, DArray_compare cmp)
{
    if (p < r) {
        int q = (p + r) / 2;
        DArray_merge_sort(array, p, q, cmp);
        DArray_merge_sort(array, q+1, r, cmp);
        DArray_merge(array, p, q, r, cmp);
    }
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
#ifdef DASORT
    DArray_merge_sort(array, 0, DArray_count(array) - 1, cmp);
    return 0;
#else
    return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
#endif
}

int DArray_sort_add(DArray *array, void *el, DArray_compare cmp)
{
    int result = DArray_push(array, el);
    int rc = DArray_qsort(array, cmp);
    return result | rc;
}

int DArray_find(DArray *array, void *el, DArray_compare cmp)
{
    // DArray has to be sorted
    int low = 0;
    int high = DArray_count(array) - 1;

    while (low <= high) {
        int middle = low + (high - low) / 2;
        void *value = array->contents[middle];

        if (cmp(&el, &value) < 0) {
            high = middle - 1;
        } else if (cmp(&el, &value) > 0) {
            low = middle + 1;
        } else {
            return middle;
        }
    }

    return -1;
}