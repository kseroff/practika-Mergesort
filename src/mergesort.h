#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdbool.h>

#define MAX_SIZE 10000
#define MIN_SIZE 1

typedef struct {
    int comparisons;
    int copies;
    int recursiveCalls;
    int arraySize;
    double timeMs;
} MergeSortMetrics;

// Глобальный массив
extern int currentArray[];
extern int currentSize;
extern bool arrayLoaded;

void mergeSort(int arr[], int left, int right, MergeSortMetrics* metrics);
void merge(int arr[], int left, int mid, int right, MergeSortMetrics* metrics);
void printMetrics(MergeSortMetrics metrics);

// Функции для консольного интерфейса
void showMenu();
void createArrayManually();
void generateRandomArray();
void loadArrayFromFile();
void sortCurrentArray();
void printCurrentArray();
void clearArray();

// Функции валидации
bool validateSize(int size);
bool validateFileExists(const char* filename);
bool validateFileContent(const char* filename);
bool validateNumericInput(const char* str);
void clearInputBuffer();

#endif