typedef struct {
	float dados[10];
	int qtd;
} Sensor1D;

void s_init(Sensor1D* s);
void s_add(Sensor1D* s, float valor);
float s_media(const Sensor1D* s);
float s_max(const Sensor1D* s);
void s_print(const Sensor1D* s);
