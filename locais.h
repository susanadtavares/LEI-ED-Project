#ifndef LOCAIS_H
#define LOCAIS_H

typedef struct {
    char id[3];
    char nome[50];
} Distrito;

typedef struct {
    char id[5];
    char nome[50];
} Concelho;

typedef struct {
    char id[7];
    char nome[50];
} Freguesia;

int carregarDistritos(Distrito *distritos, int *numDistritos, const char *filename);
int carregarConcelhos(Concelho *concelhos, int *numConcelhos, const char *filename);
int carregarFreguesias(Freguesia *freguesias, int *numFreguesias, const char *filename);

#endif // LOCAIS_H