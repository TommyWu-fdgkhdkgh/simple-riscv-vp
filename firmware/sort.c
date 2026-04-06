#include "sort.h"
#include "printf.h"
#include "m5ops.h"
#include "riscv.h"
#include <stdint.h>

#define ARRAY_SIZE 2000

static int test_array[ARRAY_SIZE];

static void verify_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
      if (i != 0 && arr[i] < arr[i - 1]) {
        printf("The sort algorithm is wrong!!!\n");
	return;
      }
    }
    printf("The sort algorithm is correct!!!\n");
}

static void print_array(int *arr, int size) {
    if (size <= 20) {
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
    } else {
        for (int i = 0; i < 10; i++) {
            printf("%d ", arr[i]);
        }
        printf("... ");
        for (int i = size - 10; i < size; i++) {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

static void reset_array(void) {
    // Simple pseudo-random initialization
    unsigned int seed = 12345;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        seed = (seed * 1103515245 + 12345) & 0x7fffffff;
        test_array[i] = seed % 10000;
    }
}

void bubble_sort_internal(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubble_sort(void) {
    reset_array();
    printf("Before Bubble Sort: ");
    print_array(test_array, ARRAY_SIZE);

    /***************/
    /* Start timer */
    /***************/
    uint64_t begin_mtime = 0;
    uint64_t end_mtime = 0;
    uint64_t use_mtime = 0;
    uint32_t tmp_mtime = 0;
    begin_mtime = r_mtime64();
    m5_resetstats();

    /**************/
    /* run sort   */
    /**************/
    bubble_sort_internal(test_array, ARRAY_SIZE);

    /**************/
    /* Stop timer */
    /**************/
    m5_dumpstats();
    end_mtime = r_mtime64();
    use_mtime = end_mtime - begin_mtime;
    tmp_mtime = end_mtime;
    tmp_mtime = use_mtime;
    printf("use_mtime_low : %u\n", tmp_mtime);
    tmp_mtime = use_mtime >> 32;
    printf("use_mtime_high : %u\n", tmp_mtime);

    printf("After Bubble Sort:  ");
    print_array(test_array, ARRAY_SIZE);
    verify_array(test_array, ARRAY_SIZE);
}

static int L[ARRAY_SIZE / 2 + 1];
static int R[ARRAY_SIZE / 2 + 1];

static void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

static void merge_sort_recursive(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort_recursive(arr, left, mid);
        merge_sort_recursive(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void merge_sort(void) {
    reset_array();
    printf("Before Merge Sort: ");
    print_array(test_array, ARRAY_SIZE);

    /***************/
    /* Start timer */
    /***************/
    uint64_t begin_mtime = 0;
    uint64_t end_mtime = 0;
    uint64_t use_mtime = 0;
    uint32_t tmp_mtime = 0;
    begin_mtime = r_mtime64();
    m5_resetstats();

    /**************/
    /* run sort   */
    /**************/
    merge_sort_recursive(test_array, 0, ARRAY_SIZE - 1);

    /**************/
    /* Stop timer */
    /**************/
    m5_dumpstats();
    end_mtime = r_mtime64();
    use_mtime = end_mtime - begin_mtime;
    tmp_mtime = end_mtime;
    tmp_mtime = use_mtime;
    printf("use_mtime_low : %u\n", tmp_mtime);
    tmp_mtime = use_mtime >> 32;
    printf("use_mtime_high : %u\n", tmp_mtime);

    printf("After Merge Sort:  ");
    print_array(test_array, ARRAY_SIZE);
    verify_array(test_array, ARRAY_SIZE);
}
