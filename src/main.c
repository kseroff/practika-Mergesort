#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mergesort.h"

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void copyArray(int source[], int dest[], int size) {
    for (int i = 0; i < size; i++)
        dest[i] = source[i];
}

void runSortTest(int original[], int size, char* testName) {
    printf("\n========== %s ==========\n", testName);

    if (size <= 20) {
        printf("Исходный массив: ");
        printArray(original, size);
    }

    // Исправлено: проверка malloc
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        return;
    }
    copyArray(original, arr, size);

    MergeSortMetrics metrics = {0, 0, 0, size, 0.0};

    clock_t start = clock();
    mergeSort(arr, 0, size - 1, &metrics);
    clock_t end = clock();
    metrics.timeMs = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    if (size <= 20) {
        printf("Отсортированный: ");
        printArray(arr, size);
    }

    printMetrics(metrics);
    free(arr);
}

int main() {
    int arr1[] = {38, 27, 43, 3, 9, 82, 10};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    runSortTest(arr1, size1, "Обычный массив");

    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    runSortTest(arr2, size2, "Отсортированный массив");

    int arr3[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    runSortTest(arr3, size3, "Обратный порядок");

    int arr4[] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    runSortTest(arr4, size4, "Одинаковые значения");

    int arr5[] = {-5, 3, -8, 0, -2, 7, -1, 4, -3, 6};
    int size5 = sizeof(arr5) / sizeof(arr5[0]);
    runSortTest(arr5, size5, "Отрицательные числа");

    int arr6[] = {42};
    int size6 = sizeof(arr6) / sizeof(arr6[0]);
    runSortTest(arr6, size6, "Один элемент");

    // Исправлено: проверка malloc для большого массива
    int bigSize = 1000;
    int* bigArr = (int*)malloc(bigSize * sizeof(int));
    if (bigArr == NULL) {
        printf("Ошибка: не удалось выделить память для большого массива!\n");
        return 1;
    }
    for (int i = 0; i < bigSize; i++) {
        bigArr[i] = rand() % 10000;
    }
    runSortTest(bigArr, bigSize, "Большой массив (1000 элементов)");
    free(bigArr);

    return 0;
}