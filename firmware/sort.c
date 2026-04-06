#include "sort.h"
#include "printf.h"

#define ARRAY_SIZE 10

static int test_array[ARRAY_SIZE] = {42, 12, 89, 27, 11, 56, 3, 78, 34, 19};

static void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static void reset_array(void) {
    int original[ARRAY_SIZE] = {42, 12, 89, 27, 11, 56, 3, 78, 34, 19};
    for (int i = 0; i < ARRAY_SIZE; i++) {
        test_array[i] = original[i];
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
    bubble_sort_internal(test_array, ARRAY_SIZE);
    printf("After Bubble Sort:  ");
    print_array(test_array, ARRAY_SIZE);
}

static void merge(int *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[10], R[10]; // Assuming size <= 10 for simplicity in this firmware

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
    merge_sort_recursive(test_array, 0, ARRAY_SIZE - 1);
    printf("After Merge Sort:  ");
    print_array(test_array, ARRAY_SIZE);
}
