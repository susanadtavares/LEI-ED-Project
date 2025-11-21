#ifndef AREAS_H
#define AREAS_H

typedef struct Area {
    char nome[100];
    struct Area *proxima;
} Area;

int adicionarArea(Area **areas, const char *nome);  // Retorna 1 em caso de sucesso, 0 em caso de falha
void listarAreas(Area *areas);
int areaExiste(Area *areas, const char *nome);
int removerArea(Area **areas, const char *nome);

#endif