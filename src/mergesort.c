#define _CRT_SECURE_NO_WARNINGS
#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int currentArray[MAX_SIZE];
int currentSize = 0;
bool arrayLoaded = false;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

bool validateSize(int size) {
    if (size < MIN_SIZE) {
        printf("Oshibka: razmer massiva slishkom malenkiy (minimum %d).\n", MIN_SIZE);
        return false;
    }
    if (size > MAX_SIZE) {
        printf("Oshibka: razmer massiva slishkom bolshoy (maximum %d).\n", MAX_SIZE);
        return false;
    }
    return true;
}

bool validateFileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Oshibka: fail '%s' ne nayden ili ne mozhet byt otkryt.\n", filename);
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
            printf("Oshibka: v faile slishkom mnogo chisel (maximum %d).\n", MAX_SIZE);
            fclose(file);
            return false;
        }
    }

    fclose(file);

    if (!hasValidNumber || count == 0) {
        printf("Oshibka: fail ne soderzhit chislovyh dannyh ili pust.\n");
        return false;
    }

    if (count < MIN_SIZE) {
        printf("Oshibka: v faile slishkom malo chisel (minimum %d, naydeno %d).\n", MIN_SIZE, count);
        return false;
    }

    return true;
}

bool validateNumericInput(const char* str) {
    if (str == NULL || strlen(str) == 0) return false;
    int i = 0;
    if (str[0] == '-') i = 1;
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

void merge(int arr[], int left, int mid, int right, MergeSortMetrics* metrics) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* leftArr = (int*)malloc(n1 * sizeof(int));
    int* rightArr = (int*)malloc(n2 * sizeof(int));

    if (leftArr == NULL || rightArr == NULL) {
        printf("Oshibka: ne udalos vydelit pamyat!\n");
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
        }
        else {
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
    printf("\nMETRIKI MERGESORT:\n");
    printf("   Sravneniy:           %d\n", metrics.comparisons);
    printf("   Kopirovaniy:         %d\n", metrics.copies);
    printf("   Rekursivnyh vyzovov: %d\n", metrics.recursiveCalls);
    printf("   Razmer massiva:      %d\n", metrics.arraySize);
    printf("   Vremya vypolneniya:  %.3f ms\n", metrics.timeMs);
}

void printArray(int arr[], int size) {
    if (size == 0) {
        printf("   (massiv pust)\n");
        return;
    }
    if (size > 20) {
        for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
        printf("... ");
        for (int i = size - 10; i < size; i++) printf("%d ", arr[i]);
        printf("\n   (pokazany pervye 10 i poslednie 10 iz %d elementov)", size);
    }
    else {
        for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    }
    printf("\n");
}

void saveArrayToFile() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Net dannyh dlya sohraneniya!\n");
        return;
    }

    char filename[100];
    printf("Vvedite imya faila dlya sohraneniya (naprimer, output.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        printf("Oshibka: pustoе imya faila.\n");
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Oshibka: ne udalos sozdat fail '%s'.\n", filename);
        return;
    }

    fprintf(file, "=== Otsortirovannyy massiv ===\n");
    fprintf(file, "Razmer: %d elementov\n", currentSize);
    fprintf(file, "Elementy:\n");

    for (int i = 0; i < currentSize; i++) {
        fprintf(file, "%d", currentArray[i]);
        if ((i + 1) % 10 == 0) fprintf(file, "\n");
        else if (i < currentSize - 1) fprintf(file, " ");
    }
    fprintf(file, "\n=== Konets faila ===\n");

    fclose(file);
    printf("Massiv sohranyon v fail '%s' (%d elementov).\n", filename, currentSize);
}

void loadArrayFromFileWithPath() {
    char filename[100];

    printf("Vvedite put k failu (naprimer, input.txt ili C:/data/input.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        printf("Oshibka: pustoе imya faila.\n");
        return;
    }

    if (!validateFileExists(filename)) return;
    if (!validateFileContent(filename)) return;

    clearArray();

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Oshibka: ne udalos otkryt fail '%s'.\n", filename);
        return;
    }

    int size = 0;
    while (size < MAX_SIZE && fscanf(file, "%d", &currentArray[size]) == 1) {
        size++;
    }
    fclose(file);

    currentSize = size;
    arrayLoaded = true;
    printf("Zagruzheno %d elementov iz faila '%s'.\n", size, filename);
}

void showMenu() {
    printf("\n========================================\n");
    printf("     MERGESORT - KONSONLYNY INTERFEYS\n");
    printf("========================================\n");
    printf("  1. Sozdat massiv vruchnuyu\n");
    printf("  2. Sgenerirovat sluchaynyy massiv\n");
    printf("  3. Zagruzit massiv iz faila\n");
    printf("  4. Sortirovat tekushchiy massiv\n");
    printf("  5. Vyvesti tekushchiy massiv\n");
    printf("  6. Sohranit massiv v fail\n");
    printf("  7. Zagruzit massiv po puti\n");
    printf("  8. Ochistit massiv\n");
    printf("  0. Vyyty\n");
    printf("========================================\n");
    printf("Tekushchiy massiv: ");
    if (arrayLoaded && currentSize > 0) printf("%d elementov", currentSize);
    else printf("(pusto)");
    printf("\nVyberite deystvie: ");
}

void clearArray() {
    currentSize = 0;
    arrayLoaded = false;
    printf("Massiv ochishchen.\n");
}

void createArrayManually() {
    char input[100];
    int size;

    printf("Vvedite razmer massiva (%d-%d): ", MIN_SIZE, MAX_SIZE);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Убираем \n

    if (strlen(input) == 0) {
        printf("Oshibka: pustoy vvod. Poprobuyte snova.\n");
        return;
    }

    if (!validateNumericInput(input)) {
        printf("Oshibka: vvedite tselye chislo.\n");
        return;
    }

    size = atoi(input);

    if (!validateSize(size)) {
        return;
    }

    clearArray();
    currentSize = size;
    arrayLoaded = true;

    printf("Vvedite %d tselyh chisel cherez probel: ", size);

    int count = 0;
    while (count < size) {
        if (scanf("%d", &currentArray[count]) != 1) {
            printf("Oshibka vvoda! Ozhidalos chislo.\n");
            clearInputBuffer();
            return;
        }
        count++;
    }
    clearInputBuffer();

    printf("Massiv sozdan.\n");
}

void generateRandomArray() {
    char input[100];
    int size;

    printf("Vvedite razmer massiva (%d-%d): ", MIN_SIZE, MAX_SIZE);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Убираем \n

    if (strlen(input) == 0) {
        printf("Oshibka: pustoy vvod. Poprobuyte snova.\n");
        return;
    }

    if (!validateNumericInput(input)) {
        printf("Oshibka: vvedite tselye chislo.\n");
        return;
    }

    size = atoi(input);

    if (!validateSize(size)) {
        return;
    }

    clearArray();
    currentSize = size;
    arrayLoaded = true;

    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        currentArray[i] = rand() % 20001 - 10000;
    }

    printf("Sgenerirovan sluchaynyy massiv iz %d elementov.\n", size);
}

void loadArrayFromFile() {
    char filename[100];

    printf("Vvedite imya faila (naprimer, input.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        printf("Oshibka: pustoе imya faila.\n");
        return;
    }

    if (!validateFileExists(filename)) return;
    if (!validateFileContent(filename)) return;

    clearArray();

    FILE* file = fopen(filename, "r");
    int size = 0;
    while (size < MAX_SIZE && fscanf(file, "%d", &currentArray[size]) == 1) {
        size++;
    }
    fclose(file);

    currentSize = size;
    arrayLoaded = true;
    printf("Zagruzheno %d elementov iz faila '%s'.\n", size, filename);
}

void sortCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Net dannyh dlya sortirovki!\n");
        return;
    }

    int* arrCopy = (int*)malloc(currentSize * sizeof(int));
    if (arrCopy == NULL) {
        printf("Oshibka: ne udalos vydelit pamyat!\n");
        return;
    }

    for (int i = 0; i < currentSize; i++) arrCopy[i] = currentArray[i];

    MergeSortMetrics metrics = { 0, 0, 0, currentSize, 0.0 };

    clock_t start = clock();
    mergeSort(arrCopy, 0, currentSize - 1, &metrics);
    clock_t end = clock();
    metrics.timeMs = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    for (int i = 0; i < currentSize; i++) currentArray[i] = arrCopy[i];
    free(arrCopy);

    printf("Massiv otsortirovan!\n");
    printMetrics(metrics);
}

void printCurrentArray() {
    if (!arrayLoaded || currentSize == 0) {
        printf("Net dannyh dlya vyvoda!\n");
        return;
    }

    printf("\nTekushchiy massiv (%d elementov):\n", currentSize);
    printf("   ");
    printArray(currentArray, currentSize);
}