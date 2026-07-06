#include <stdio.h>
#include "mergesort.h"

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    // Обычный массив
    int arr1[] = {38, 27, 43, 3, 9, 82, 10};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    
    printf("Обычный массив:\n");
    printf("Исходный: ");
    printArray(arr1, size1);
    mergeSort(arr1, 0, size1 - 1);
    printf("Отсортированный: ");
    printArray(arr1, size1);
    printf("\n");

    // Уже отсортированный массив
    int arr2[] = {1, 2, 3, 4, 5, 6, 7};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    
    printf("Отсортированный массив:\n");
    printf("Исходный: ");
    printArray(arr2, size2);
    mergeSort(arr2, 0, size2 - 1);
    printf("Отсортированный: ");
    printArray(arr2, size2);
    printf("\n");

    // Массив в обратном порядке
    int arr3[] = {9, 8, 7, 6, 5, 4, 3};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    
    printf("Массив в обратном порядке:\n");
    printf("Исходный: ");
    printArray(arr3, size3);
    mergeSort(arr3, 0, size3 - 1);
    printf("Отсортированный: ");
    printArray(arr3, size3);
    printf("\n");

    // Массив с одинаковыми значениями
    int arr4[] = {5, 5, 5, 5, 5, 5, 5};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    
    printf("Массив с одинаковыми значениями:\n");
    printf("Исходный: ");
    printArray(arr4, size4);
    mergeSort(arr4, 0, size4 - 1);
    printf("Отсортированный: ");
    printArray(arr4, size4);
    printf("\n");

    // Массив с отрицательными числами
    int arr5[] = {-5, 3, -8, 0, -2, 7, -1};
    int size5 = sizeof(arr5) / sizeof(arr5[0]);
    
    printf("Массив с отрицательными числами:\n");
    printf("Исходный: ");
    printArray(arr5, size5);
    mergeSort(arr5, 0, size5 - 1);
    printf("Отсортированный: ");
    printArray(arr5, size5);
    printf("\n");

    // Массив из одного элемента
    int arr6[] = {42};
    int size6 = sizeof(arr6) / sizeof(arr6[0]);
    
    printf("Массив из одного элемента:\n");
    printf("Исходный: ");
    printArray(arr6, size6);
    mergeSort(arr6, 0, size6 - 1);
    printf("Отсортированный: ");
    printArray(arr6, size6);
    printf("\n");

    return 0;
}