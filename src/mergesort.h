#ifndef MERGESORT_H
#define MERGESORT_H

typedef struct {
    int comparisons;      // количество сравнений
    int copies;           // количество копирований
    int recursiveCalls;   // количество рекурсивных вызовов
    int arraySize;        // размер массива
    double timeMs;        // время выполнения в миллисекундах
} MergeSortMetrics;

void mergeSort(int arr[], int left, int right, MergeSortMetrics* metrics);
void merge(int arr[], int left, int mid, int right, MergeSortMetrics* metrics);
void printMetrics(MergeSortMetrics metrics);

#endif