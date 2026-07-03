#include <stdio.h>
#include <stdlib.h>
#include "mergesort.h"

int main() {
    int choice;
    
    printf("╔══════════════════════════════════════════╗\n");
    printf("║       ДОБРО ПОЖАЛОВАТЬ В MERGESORT!    ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    
    while (1) {
        showMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createArrayManually();
                break;
                
            case 2:
                generateRandomArray();
                break;
                
            case 3:
                loadArrayFromFile();
                break;
                
            case 4:
                sortCurrentArray();
                break;
                
            case 5:
                printCurrentArray();
                break;
                
            case 6:
                clearArray();
                break;
                
            case 0:
                printf("\n👋 До свидания!\n");
                return 0;
                
            default:
                printf("❌ Неверный выбор! Попробуйте снова.\n");
                break;
        }
    }
    
    return 0;
}