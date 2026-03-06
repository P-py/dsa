#define READING_SIZE 10

void s_init(Sensor1D* s) {
    (*s).qtd = 0;
}

void s_add(Sensor1D* s, float valor) {
    if ((*s).qtd < READING_SIZE) { 
        (*s).dados[(*s).qtd] = valor;
        (*s).qtd++;
    } else {
        printf("\nSensor atingiu a quantidade maxima de leituras!\n");
    }
}

float s_media(const Sensor1D* s) { 
    float sum = 0;
    int size = (*s).qtd;

    if (size == 0) {
        return 0.0;
    }

    for (i=0; i<size; i++) { 
        sum += (*s).dados[i]; 
    }

    return sum/READING_SIZE;
}

float s_max(const Sensor1D* s) {
    int size = (*s).qtd;

    if (size == 0) {
        return 0.0;
    }

    float maior = -1.0;
    for (int i=0; i<size; i++) {
        if (maior == -1) {
            maior = (*s).dados[i];
        } else if ((*s).dados[i] > maior) {
            maior = (*s).dados[i];
        }
    }

    return maior;
}

void s_print(const Sensor1D* s) {
    int size = (*s).qtd;
    printf("\nLeituras: \n");

    for (int i = 0; i<size; i++) {
        printf("\n%.2f", (*s).dados[i]);
    }
}