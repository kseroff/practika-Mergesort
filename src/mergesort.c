#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int left, int mid, int right, MergeSortMetrics* metrics) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* leftArr = (int*)malloc(n1 * sizeof(int));
    int* rightArr = (int*)malloc(n2 * sizeof(int));

    if (leftArr == NULL || rightArr == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        free(leftArr);
        free(rightArr);
        return;
    }

    if (leftArr == NULL || rightArr == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        free(leftArr);
        free(rightArr);
        return;
    }

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
        metrics->copies++;
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
        metrics->copies++;
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        metrics->comparisons++;
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
            metrics->copies++;
        } else {
            arr[k] = rightArr[j];
            j++;
            metrics->copies++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
        metrics->copies++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
        metrics->copies++;
    }

    free(leftArr);
    free(rightArr);
}

void mergeSort(int arr[], int left, int right, MergeSortMetrics* metrics) {
    metrics->recursiveCalls++;

    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, metrics);
    mergeSort(arr, mid + 1, right, metrics);
    merge(arr, left, mid, right, metrics);
}

void printMetrics(MergeSortMetrics metrics) {
    printf("\nМЕТРИКИ MERGESORT:\n");
    printf("   Сравнений:            %d\n", metrics.comparisons);
    printf("   Копирований:          %d\n", metrics.copies);
    printf("   Рекурсивных вызовов:  %d\n", metrics.recursiveCalls);
    printf("   Размер массива:       %d\n", metrics.arraySize);
    printf("   Время выполнения:     %.3f мс\n", metrics.timeMs);
}