#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


// Глобальные переменные
int currentArray[MAX_SIZE];
int currentSize = 0;
bool arrayLoaded = false;

// ===== ФУНКЦИИ ВАЛИДАЦИИ =====

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

bool validateSize(int size) {
    if (size < MIN_SIZE) {
        printf("Ошибка: размер массива слишком маленький (минимум %d).\n", MIN_SIZE);
        return false;
    }
    if (size > MAX_SIZE) {
        printf("Ошибка: размер массива слишком большой (максимум %d).\n", MAX_SIZE);
        return false;
    }
    return true;
}

bool validateFileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: файл '%s' не найден или не может быть открыт.\n", filename);
        return false;
    }
    fclose(file);
    return true;
}

bool validateFileContent(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return false;
    
    int num;
    int count = 0;
    int result;
    bool hasValidNumber = false;
    
    while ((result = fscanf(file, "%d", &num)) != EOF) {
        if (result == 0) {
            fgetc(file);
            continue;
        }
        hasValidNumber = true;
        count++;
        if (count > MAX_SIZE) {
            printf("Ошибка: в файле слишком много чисел (максимум %d).\n", MAX_SIZE);
            fclose(file);
            return false;
        }
    }
    
    fclose(file);
    
    if (!hasValidNumber || count == 0) {
        printf("Ошибка: файл не содержит числовых данных или пуст.\n");
        return false;
    }
    
    if (count < MIN_SIZE) {
        printf("Ошибка: в файле слишком мало чисел (минимум %d, найдено %d).\n", MIN_SIZE, count);
        return false;
    }
    
    return true;
}

bool validateNumericInput(const char* str) {
    if (str == NULL || strlen(str) == 0) {
        return false;
    }
    
    int i = 0;
    if (str[0] == '-') i = 1;
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// ===== ОСНОВНОЙ КОД MERGESORT =====

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

// ===== НОВЫЕ ФУНКЦИИ ДЛЯ РАБОТЫ С ФАЙЛАМИ =====

void saveArrayToFile() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Нет данных для сохранения! Сначала создайте или загрузите массив.\n");
        return;
    }
    
    char filename[100];
    printf("Введите имя файла для сохранения (например, output.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Ошибка: пустое имя файла.\n");
        return;
    }
    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл '%s'.\n", filename);
        return;
    }
    
    fprintf(file, "=== Отсортированный массив ===\n");
    fprintf(file, "Размер: %d элементов\n", currentSize);
    fprintf(file, "Элементы:\n");
    
    for (int i = 0; i < currentSize; i++) {
        fprintf(file, "%d", currentArray[i]);
        if ((i + 1) % 10 == 0) {
            fprintf(file, "\n");
        } else if (i < currentSize - 1) {
            fprintf(file, " ");
        }
    }
    fprintf(file, "\n");
    fprintf(file, "=== Конец файла ===\n");
    
    fclose(file);
    printf("Массив сохранён в файл '%s' (%d элементов).\n", filename, currentSize);
}

void loadArrayFromFileWithPath() {
    char filename[100];
    
    printf("Введите путь к файлу (например, input.txt или C:/data/input.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Ошибка: пустое имя файла.\n");
        return;
    }
    
    if (!validateFileExists(filename)) {
        return;
    }
    
    if (!validateFileContent(filename)) {
        return;
    }
    
    clearArray();
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: не удалось открыть файл '%s'.\n", filename);
        return;
    }
    
    int size = 0;
    while (size < MAX_SIZE && fscanf(file, "%d", &currentArray[size]) == 1) {
        size++;
    }
    fclose(file);
    
    currentSize = size;
    arrayLoaded = true;
    printf("Загружено %d элементов из файла '%s'.\n", size, filename);
}

// ===== ФУНКЦИИ ИНТЕРФЕЙСА =====

void showMenu() {
    printf("\n========================================\n");
    printf("     MERGESORT - КОНСОЛЬНЫЙ ИНТЕРФЕЙС\n");
    printf("========================================\n");
    printf("  1. Создать массив вручную\n");
    printf("  2. Сгенерировать случайный массив\n");
    printf("  3. Загрузить массив из файла\n");
    printf("  4. Сортировать текущий массив\n");
    printf("  5. Вывести текущий массив\n");
    printf("  6. Сохранить массив в файл\n");
    printf("  7. Загрузить массив по пути\n");
    printf("  8. Очистить массив\n");
    printf("  0. Выйти\n");
    printf("========================================\n");
    printf("Текущий массив: ");
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
    printf("Массив очищен.\n");
}

void createArrayManually() {
    char input[100];
    int size;
    
    printf("Введите размер массива (%d-%d): ", MIN_SIZE, MAX_SIZE);
    fgets(input, sizeof(input), stdin);
    
    if (input[0] == '\n') {
        printf("Ошибка: пустой ввод. Попробуйте снова.\n");
        return;
    }
    
    if (!validateNumericInput(input)) {
        printf("Ошибка: введите целое число.\n");
        return;
    }
    
    size = atoi(input);
    
    if (!validateSize(size)) {
        return;
    }
    
    clearArray();
    currentSize = size;
    arrayLoaded = true;
    
    printf("Введите %d целых чисел через пробел: ", size);
    
    int count = 0;
    while (count < size) {
        scanf("%d", &currentArray[count]);
        count++;
    }
    clearInputBuffer();
    
    printf("Массив создан.\n");
}

void generateRandomArray() {
    char input[100];
    int size;
    
    printf("Введите размер массива (%d-%d): ", MIN_SIZE, MAX_SIZE);
    fgets(input, sizeof(input), stdin);
    
    if (input[0] == '\n') {
        printf("Ошибка: пустой ввод. Попробуйте снова.\n");
        return;
    }
    
    if (!validateNumericInput(input)) {
        printf("Ошибка: введите целое число.\n");
        return;
    }
    
    size = atoi(input);
    
    if (!validateSize(size)) {
        return;
    }
    
    clearArray();
    currentSize = size;
    arrayLoaded = true;
    
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        currentArray[i] = rand() % 20001 - 10000;
    }
    
    printf("Сгенерирован случайный массив из %d элементов.\n", size);
}

void loadArrayFromFile() {
    char filename[100];
    
    printf("Введите имя файла (например, input.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        printf("Ошибка: пустое имя файла.\n");
        return;
    }
    
    if (!validateFileExists(filename)) {
        return;
    }
    
    if (!validateFileContent(filename)) {
        return;
    }
    
    clearArray();
    
    FILE* file = fopen(filename, "r");
    int size = 0;
    while (size < MAX_SIZE && fscanf(file, "%d", &currentArray[size]) == 1) {
        size++;
    }
    fclose(file);
    
    currentSize = size;
    arrayLoaded = true;
    printf("Загружено %d элементов из файла '%s'.\n", size, filename);
}

void sortCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Нет данных для сортировки! Сначала создайте или загрузите массив.\n");
        return;
    }
    
    int* arrCopy = (int*)malloc(currentSize * sizeof(int));
    if (arrCopy == NULL) {
        printf("Ошибка: не удалось выделить память!\n");
        return;
    }
    
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
    
    printf("Массив отсортирован.\n");
    printMetrics(metrics);
}

void printCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Нет данных для вывода! Сначала создайте или загрузите массив.\n");
        return;
    }
    
    printf("\nТекущий массив (%d элементов):\n", currentSize);
    printf("   ");
    printArray(currentArray, currentSize);
}