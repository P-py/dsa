#include <stdio.h>
#include "sensor1d.h"

#define READING_SIZE 10

void s_init(Sensor1D* s) {
    (*s).qtd = 0;
}

void s_add(Sensor1D* s, float valor) {
    int size = (*s).qtd;
    
    if (size < READING_SIZE) {
        (*s).dados[size] = valor;
        (*s).qtd++;
    }
}

float s_media(const Sensor1D* s) {
    float sum = 0.0;
    int size = (*s).qtd;

    for (int i=0; i<size; i++) {
        sum += (*s).dados[i];
    }
    return sum/size;
}

float s_max(const Sensor1D* s) {
    int size = (*s).qtd;

    if (size == 0) {
        return 0.0;
    }

    float max = (*s).dados[0];

    for (int i=1; i<size; i++) {
        if ((*s).dados[i] > max) {
            max = (*s).dados[i];
        }
    }
    return max;
}

void s_print(const Sensor1D* s) {
    int size = (*s).qtd;

    for (int i=0; i<size; i++) {
        printf("Leitura %d: %.2f\n", (i+1), (*s).dados[i]);
    }
}