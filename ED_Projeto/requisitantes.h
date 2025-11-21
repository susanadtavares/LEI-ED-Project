#ifndef REQUISITANTES_H
#define REQUISITANTES_H

typedef struct Requisitante {
    char id_requisitante[10];
    char nome[100];
    char data_nasc[11];
    char id_freguesia[7];
    struct Requisitante *prox;
} Requisitante;

Requisitante *carregarRequisitantes(const char *filename);
void salvarRequisitantes(Requisitante *lista, const char *filename);
void liberarRequisitantes(Requisitante *lista);
void listarRequisitantes(Requisitante *lista);
void adicionarRequisitante(Requisitante **lista);
void removerRequisitante(Requisitante **lista, const char *id);
int compararStringsInsensivel(const char *str1, const char *str2);
void pesquisarRequisitante(Requisitante *lista, const char *nome);
Requisitante* copiarListaRequesitantes(Requisitante *requisitantes);
Requisitante* ordenarPorNome(Requisitante *requisitantes);
Requisitante* ordenarPorIdFreguesia(Requisitante *requisitantes);
Requisitante* ordenarPorApelido(Requisitante *requisitantes);
Requisitante* ordenarListaPorApelido(Requisitante *requisitantes);
void listarIdadeMaisRequisitantes(Requisitante *lista);
void mediaIdadesRequisitantes(Requisitante *lista);
void idadeMaximaRequisitantes(Requisitante *lista);

char *extrairNome(Requisitante *requisitante);
char *extrairPrimeiroNome(Requisitante *requisitante);
char *extrairSobrenome(Requisitante *requisitante);
char *extrairFreguesia(Requisitante *requisitante);
char *extrairFreguesia(Requisitante *requisitante);
void mostrarEstatisticas(Requisitante *lista);
void toUpperCase(char *str);

#endif // REQUISITANTES_H
