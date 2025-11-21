#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include "livros.h"
#include "requisitantes.h"
#include "locais.h"
#include "areas.h"
#include <stddef.h>

typedef struct {
    Livro *livros;
    Requisitante *requisitantes;
    Area *areas;
    Distrito distritos[100];
    int numDistritos;
    Concelho concelhos[300];
    int numConcelhos;
    Freguesia freguesias[1000];
    int numFreguesias;
} Biblioteca;

void log_error(const char *error_message);

void inicializarBiblioteca(Biblioteca *biblioteca);
void carregarDadosBiblioteca(Biblioteca *biblioteca);
void salvarDadosBiblioteca(Biblioteca *biblioteca);
void CarregarBiblioteca(const char *nomeArquivo, Biblioteca *bib);
void Carregarlivros(const char *nomeArquivo, Livro *liv);
void CarregarRequisitantes(const char *nomeArquivo, Requisitante *rq);
void CarregarAreas(const char *nomeArquivo, Area *area);
void liberarBiblioteca(Biblioteca *biblioteca);
size_t calcularMemoriaBiblioteca(Biblioteca *biblioteca);
void menu(Biblioteca *biblioteca);
void menuRequisicoes(Biblioteca *biblioteca);

#endif // BIBLIOTECA_H
