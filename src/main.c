#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "mergesort.h"

int main() {
    int choice;
    printf("========================================\n");
    printf("       ДОБРО ПОЖАЛОВАТЬ В MERGESORT!\n");
    printf("========================================\n");
    
    while (1) {
        showMenu();
        
        char input[100];
        fgets(input, sizeof(input), stdin);
        
        if (input[0] == '\n') {
            printf("Ошибка: пустой ввод. Выберите действие от 0 до 8.\n");
            continue;
        }
        
        if (!validateNumericInput(input)) {
            printf("Ошибка: введите число от 0 до 8.\n");
            continue;
        }
        
        choice = atoi(input);
        switch (choice) {
            case 1: createArrayManually(); break;
            case 2: generateRandomArray(); break;
            case 3: loadArrayFromFile(); break;
            case 4: sortCurrentArray(); break;
            case 5: printCurrentArray(); break;
            case 6: saveArrayToFile(); break;
            case 7: loadArrayFromFileWithPath(); break;
            case 8: clearArray(); break;
            case 0:
                printf("\nДо свидания!\n");
                return 0;
            default:
                printf("Ошибка: неверный выбор! Введите число от 0 до 8.\n");
                break;
        }
    }

    return 0;
}
