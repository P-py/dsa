#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bag.h"

#define EXIT_OPTION 7

void menu();
void randomValues();

void main() {
    srand(time(NULL));
    int opt = -1;
    
    Bag bag;
    init(&bag);
    printf("\nBag inicializada!\n");
    printf("O tamanho atual da bag: %d\n\n", size(&bag));

    while (opt != EXIT_OPTION) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);
        switch(opt) {
            case 1:
                system("clear");
                printf("\nO tamanho atual da bag: %d\n\n", size(&bag));
                break;
            case 2:
                system("clear");
                randomValues(&bag);
                break;
            default: break;
        }
    }
}

void randomValues(Bag bag) {
    for (int i=0; i<BAGSIZE; i++) {
        int randomNum = rand() % 1000;
        
        insert(&bag, randomNum);
    }
    printall(&bag);
    printf("\nForam inseridos %d valores aleatorios na bag\n", BAGSIZE);
    printf("O tamanho atual da bag: %d\n\n", size(&bag));
}

void menu() {
    printf("1. Exibir o tamanho da bag\n");
    printf("2. Inserir valores aleatórios na bag\n");
    printf("3. Inserir um valor na bag\n");
    printf("4. Verificar se um valor existe na bag\n");
    printf("5. Remover um valor da bag\n");
    printf("6. Exibir todos os valores da bag\n");
    printf("7. Sair\n");
}