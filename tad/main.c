#include <stdio.h>
#include "sensor1d.h"
int main() {
    Sensor1D s;
    s_init(&s);
    
    float leitura;
    for (int i = 0; i < 10; i++) {
        printf("Digite a leitura %d: ", i);
        scanf("%f", &leitura);
        // adicione o valor lido no Sensor1D
    }
    printf("\nLeituras registradas:\n");
    // imprimir os valores armazenados no Sensor1D
    printf("\nMedia = %.2f\n",               ); // utilize a função correta do Sensor1D
    printf("Maximo = %.2f\n",                );// utilize a função correta do Sensor1D
    return 0;
}