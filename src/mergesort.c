#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Глобальные переменные
int currentArray[MAX_SIZE];
int currentSize = 0;
bool arrayLoaded = false;

void merge(int arr[], int left, int mid, int right, MergeSortMetrics* metrics) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* leftArr = (int*)malloc(n1 * sizeof(int));
    int* rightArr = (int*)malloc(n2 * sizeof(int));

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
    printf("\n📊 МЕТРИКИ MERGESORT:\n");
    printf("   Сравнений:            %d\n", metrics.comparisons);
    printf("   Копирований:          %d\n", metrics.copies);
    printf("   Рекурсивных вызовов:  %d\n", metrics.recursiveCalls);
    printf("   Размер массива:       %d\n", metrics.arraySize);
    printf("   Время выполнения:     %.3f мс\n", metrics.timeMs);
}

void printArray(int arr[], int size) {
    if (size == 0) {
        printf("   (массив пуст)\n");
        return;
    }
    
    if (size > 20) {
        for (int i = 0; i < 10; i++)
            printf("%d ", arr[i]);
        printf("... ");
        for (int i = size - 10; i < size; i++)
            printf("%d ", arr[i]);
        printf("\n   (показаны первые 10 и последние 10 из %d элементов)", size);
    } else {
        for (int i = 0; i < size; i++)
            printf("%d ", arr[i]);
    }
    printf("\n");
}

void showMenu() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║     MERGESORT - КОНСОЛЬНЫЙ ИНТЕРФЕЙС    ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  1. Создать массив вручную              ║\n");
    printf("║  2. Сгенерировать случайный массив      ║\n");
    printf("║  3. Загрузить массив из файла           ║\n");
    printf("║  4. Сортировать текущий массив          ║\n");
    printf("║  5. Вывести текущий массив              ║\n");
    printf("║  6. Очистить массив                     ║\n");
    printf("║  0. Выйти                               ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\nТекущий массив: ");
    if (arrayLoaded && currentSize > 0) {
        printf("%d элементов", currentSize);
    } else {
        printf("(пусто)");
    }
    printf("\n");
    printf("Выберите действие: ");
}

void clearArray() {
    currentSize = 0;
    arrayLoaded = false;
    printf("✅ Массив очищен.\n");
}

void createArrayManually() {
    int size;
    printf("Введите размер массива (1-%d): ", MAX_SIZE);
    scanf("%d", &size);
    
    if (size < 1 || size > MAX_SIZE) {
        printf("❌ Неверный размер!\n");
        return;
    }
    
    clearArray();
    currentSize = size;
    arrayLoaded = true;
    
    printf("Введите %d целых чисел через пробел: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &currentArray[i]);
    }
    
    printf("✅ Массив создан!\n");
}

void generateRandomArray() {
    int size;
    printf("Введите размер массива (1-%d): ", MAX_SIZE);
    scanf("%d", &size);
    
    if (size < 1 || size > MAX_SIZE) {
        printf("❌ Неверный размер!\n");
        return;
    }
    
    clearArray();
    currentSize = size;
    arrayLoaded = true;
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        currentArray[i] = rand() % 20001 - 10000;
    }
    
    printf("✅ Сгенерирован случайный массив из %d элементов!\n", size);
}

void loadArrayFromFile() {
    char filename[100];
    printf("Введите имя файла (например, input.txt): ");
    scanf("%s", filename);
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("❌ Не удалось открыть файл '%s'!\n", filename);
        return;
    }
    
    clearArray();
    int size = 0;
    while (fscanf(file, "%d", &currentArray[size]) == 1 && size < MAX_SIZE) {
        size++;
    }
    fclose(file);
    
    if (size == 0) {
        printf("❌ Файл пуст или содержит нечисловые данные!\n");
        return;
    }
    
    currentSize = size;
    arrayLoaded = true;
    printf("✅ Загружено %d элементов из файла '%s'!\n", size, filename);
}

void sortCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("❌ Нет данных для сортировки! Сначала создайте или загрузите массив.\n");
        return;
    }
    
    int* arrCopy = (int*)malloc(currentSize * sizeof(int));
    for (int i = 0; i < currentSize; i++) {
        arrCopy[i] = currentArray[i];
    }
    
    MergeSortMetrics metrics = {0, 0, 0, currentSize, 0.0};
    
    clock_t start = clock();
    mergeSort(arrCopy, 0, currentSize - 1, &metrics);
    clock_t end = clock();
    metrics.timeMs = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    
    for (int i = 0; i < currentSize; i++) {
        currentArray[i] = arrCopy[i];
    }
    free(arrCopy);
    
    printf("✅ Массив отсортирован!\n");
    printMetrics(metrics);
}

void printCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("❌ Нет данных для вывода! Сначала создайте или загрузите массив.\n");
        return;
    }
    
    printf("\n📋 Текущий массив (%d элементов):\n", currentSize);
    printf("   ");
    printArray(currentArray, currentSize);
}