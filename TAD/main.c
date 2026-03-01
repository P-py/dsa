#include <stdio.h>
#include "sensor1d.h"

int main() {
    Sensor1D s;

    s_init(&s);

    float leitura;
    for (int i = 0; i < 10; i++) {
        printf("Digite a leitura %d: ", (i+1));
        scanf("%f", &leitura);
        s_add(&s, leitura);
    }

    printf("\nLeituras registradas:\n");
    s_print(&s);
    printf("\nMedia = %.2f\n", s_media(&s));
    printf("Maximo = %.2f\n", s_max(&s));
    return 0;
}