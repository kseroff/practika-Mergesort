#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mergesort.h"

int main() {
    int choice;

    printf("========================================\n");
    printf("       DOBRO POZHALOVAT V MERGESORT!\n");
    printf("========================================\n");

    while (1) {
        showMenu();

        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        // Удаляем символ новой строки
        input[strcspn(input, "\n")] = '\0';

        // Проверка на пустой ввод
        if (strlen(input) == 0) {
            printf("Oshibka: pustoy vvod. Vyberite deystvie ot 0 do 8.\n");
            continue;
        }

        // Проверяем, что ввод содержит только цифры
        int isNumber = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                isNumber = 0;
                break;
            }
        }

        if (!isNumber) {
            printf("Oshibka: vvedite chislo ot 0 do 8.\n");
            continue;
        }

        choice = atoi(input);

        if (choice < 0 || choice > 8) {
            printf("Oshibka: vvedite chislo ot 0 do 8.\n");
            continue;
        }

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
            printf("\nDo svidaniya!\n");
            return 0;
        default:
            printf("Oshibka: nevernyy vybor! Vvedite chislo ot 0 do 8.\n");
            break;
        }
    }

    return 0;
}