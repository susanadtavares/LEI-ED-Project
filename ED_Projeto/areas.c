#include "areas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

int adicionarArea(Area **areas, const char *nome) {
    Area *novaArea = (Area *)malloc(sizeof(Area));
    if (!novaArea) {
        fprintf(stderr, "Erro ao alocar memória para a nova área\n");
        log_error("Erro ao alocar memória para a nova área");
        return 0;  // Falha
    }
    strncpy(novaArea->nome, nome, sizeof(novaArea->nome) - 1);
    novaArea->nome[sizeof(novaArea->nome) - 1] = '\0';
    novaArea->proxima = *areas;
    *areas = novaArea;
    return 1;  // Sucesso
}

void listarAreas(Area *areas) {
    printf("\n--- Áreas Disponíveis ---\n");
    Area *atual = areas;
    while (atual != NULL) {
        printf("- %s\n", atual->nome);
        atual = atual->proxima;
    }
}

int areaExiste(Area *areas, const char *nome) {
    Area *atual = areas;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return 1; // A área existe
        }
        atual = atual->proxima;
    }
    return 0; // A área não existe
}

int removerArea(Area **areas, const char *nome) {
    Area *atual = *areas;
    Area *anterior = NULL;

    // Procurar pela área a ser removida
    while (atual != NULL && strcmp(atual->nome, nome) != 0) {
        anterior = atual;
        atual = atual->proxima;
    }

    // Se não encontrou a área, retornar 0
    if (atual == NULL) {
        return 0;
    }

    // Remover a área encontrada
    if (anterior == NULL) {
        // A área a ser removida é a primeira da lista
        *areas = atual->proxima;
    } else {
        // A área a ser removida está no meio ou fim da lista
        anterior->proxima = atual->proxima;
    }

    // Liberar a memória da área removida
    free(atual);

    return 1; // Sucesso
}