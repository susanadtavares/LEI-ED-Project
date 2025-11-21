#include <stdio.h>
#include "biblioteca.h"
#include "livros.h"
#include "requisitantes.h"
#include "areas.h"

int main() {
    Biblioteca biblioteca;
    Livro livro;
    Requisitante requisitante;
    Area area;
    CarregarBiblioteca("biblioteca.bin", &biblioteca);
    Carregarlivros("livros.bin", &livro);
    CarregarRequisitantes("requisitantes.bin", &requisitante);
    CarregarAreas("areas.bin", &area);
    inicializarBiblioteca(&biblioteca);
    carregarDadosBiblioteca(&biblioteca);
    menu(&biblioteca);
    salvarDadosBiblioteca(&biblioteca);
    liberarBiblioteca(&biblioteca);
    return 0;
}
