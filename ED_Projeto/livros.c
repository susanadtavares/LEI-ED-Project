#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "livros.h"
#include "biblioteca.h"
#include "requisitantes.h"

#define HASH_TABLE_SIZE 100

// Estrutura para um nó da lista encadeada usada na tabela hash
typedef struct HashNode {
    Livro *livro;
    struct HashNode *prox;
} HashNode;

// Tabela hash como um array de ponteiros para HashNode
HashNode *hashTable[HASH_TABLE_SIZE];

// Função hash simples para converter um ISBN em um índice na tabela hash
unsigned int hashFunction(const char *isbn) {
    unsigned int hash = 0;
    while (*isbn) {
        hash = (hash * 31) + (*isbn++);
    }
    return hash % HASH_TABLE_SIZE;
}

// Verifica se o ISBN tem entre 10 e 13 caracteres
int verificarISBN(const char *isbn) {
    int length = strlen(isbn);
    return (length >= 10 && length <= 13);
}

void listarLivros() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *atual = hashTable[i];
        while (atual != NULL) {
            printf("ISBN: %s\n", atual->livro->isbn);
            printf("Título: %s\n", atual->livro->titulo);
            printf("Autor: %s\n", atual->livro->autor);
            printf("Área: %s\n", atual->livro->area);
            printf("Ano de Publicação: %d\n", atual->livro->ano_publicacao);
            if (strlen(atual->livro->id_requisitante) > 0) {
                printf("Status: Requisitado por %s\n", atual->livro->id_requisitante);
            } else {
                printf("Status: Disponível\n");
            }
            printf("\n");
            atual = atual->prox;
        }
    }
}

// Função para remover livros nunca requisitados e listar os restantes
void removerLivrosNuncaRequisitados() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hashTable[i];
        HashNode *prev = NULL;

        while (current != NULL) {
            Livro *livro = current->livro;

            // Verifica se o livro nunca foi requisitado (contador_requisicoes == 0)
            if (livro->contador_requisicoes == 0) {
                // Livro nunca requisitado, remove da tabela hash
                if (prev == NULL) {
                    hashTable[i] = current->prox;
                } else {
                    prev->prox = current->prox;
                }

                HashNode *temp = current;
                current = current->prox;
                free(temp->livro);
                free(temp);
            } else {
                prev = current;
                current = current->prox;
            }
        }
    }

    // Após remover os livros nunca requisitados, listar os restantes
    printf("\nLivros restantes após remover os nunca requisitados:\n");
    listarLivrosRestantes();
}

// Função para listar todos os livros restantes na tabela hash
void listarLivrosRestantes() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hashTable[i];

        while (current != NULL) {
            Livro *livro = current->livro;

            printf("ISBN: %s\n", livro->isbn);
            printf("Título: %s\n", livro->titulo);
            printf("Autor: %s\n", livro->autor);
            printf("Área: %s\n", livro->area);
            printf("Ano de Publicação: %d\n", livro->ano_publicacao);
            if (strlen(livro->id_requisitante) > 0) {
                printf("Status: Requisitado por %s\n", livro->id_requisitante);
            } else {
                printf("Status: Disponível\n");
            }
            printf("Número de Requisições: %d\n", livro->contador_requisicoes);
            printf("\n");

            current = current->prox;
        }
    }
}

// Adiciona um novo livro à tabela hash
void adicionarLivro(Livro novoLivro) {
    if (!verificarISBN(novoLivro.isbn)) {
        printf("ISBN inválido: %s. O ISBN deve ter entre 10 e 13 caracteres.\n", novoLivro.isbn);
        log_error("ISBN inválido");
        return;
    }

    unsigned int index = hashFunction(novoLivro.isbn);

    // Cria um novo nó HashNode para o novo livro
    HashNode *newNode = (HashNode*) malloc(sizeof(HashNode));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para o novo livro.\n");
        return;
    }

    // Cria um novo livro e copia os dados
    Livro *novo = (Livro*) malloc(sizeof(Livro));
    if (novo == NULL) {
        free(newNode);
        printf("Erro ao alocar memória para o novo livro.\n");
        return;
    }
    *novo = novoLivro;
    novo->prox = NULL;

    novo->contador_requisicoes = 0;

    newNode->livro = novo;
    newNode->prox = hashTable[index];
    hashTable[index] = newNode;

    devolverLivro(novoLivro.isbn);
}

// Remove um livro da tabela hash pelo ISBN
void removerLivro(const char *isbn) {
    unsigned int index = hashFunction(isbn);
    HashNode *current = hashTable[index];
    HashNode *prev = NULL;

    while (current != NULL) {
        Livro *livro = current->livro;

        if (strcmp(livro->isbn, isbn) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->prox;
            } else {
                prev->prox = current->prox;
            }

            free(livro);
            free(current);
            printf("Livro com ISBN %s removido com sucesso.\n", isbn);
            return;
        }

        prev = current;
        current = current->prox;
    }

    printf("Livro com ISBN %s não encontrado para remoção.\n", isbn);
}

// Libera a memória alocada para a tabela hash e seus elementos
void liberarLivros() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hashTable[i];

        while (current != NULL) {
            HashNode *temp = current;
            current = current->prox;
            free(temp->livro);
            free(temp);
        }

        hashTable[i] = NULL;
    }
}

// Pesquisa um livro na tabela hash pelo ISBN
Livro* pesquisarLivro(const char *isbn) {
    unsigned int index = hashFunction(isbn);
    HashNode *current = hashTable[index];

    while (current != NULL) {
        Livro *livro = current->livro;

        if (strcmp(livro->isbn, isbn) == 0) {
            return livro;
        }

        current = current->prox;
    }

    return NULL;  // Livro não encontrado
}

// Função para listar livros agrupados por área
void listarLivrosAgrupadosPorArea() {
    int i;

    // Array para marcar áreas já processadas
    int areasProcessadas[HASH_TABLE_SIZE] = {0};

    // Percorre a tabela hash
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *atual = hashTable[i];

        // Percorre os elementos na lista encadeada
        while (atual != NULL) {
            Livro *livro = atual->livro;
            char *areaAtual = livro->area;

            // Verifica se a área já foi processada
            if (!areasProcessadas[hashFunction(areaAtual)]) {
                printf("\n--- Área: %s ---\n", areaAtual);
                areasProcessadas[hashFunction(areaAtual)] = 1; // Marca a área como processada
            }

            // Lista todos os livros desta área
            HashNode *temp = hashTable[i];
            while (temp != NULL) {
                Livro *livroTemp = temp->livro;
                if (strcmp(livroTemp->area, areaAtual) == 0) {
                    printf("ISBN: %s\n", livroTemp->isbn);
                    printf("Título: %s\n", livroTemp->titulo);
                    printf("Autor: %s\n", livroTemp->autor);
                    printf("Ano de Publicação: %d\n", livroTemp->ano_publicacao);
                    if (strlen(livroTemp->id_requisitante) > 0) {
                        printf("(Requisitado por %s)\n", livroTemp->id_requisitante);
                    }
                    printf("\n");
                }
                temp = temp->prox;
            }

            atual = atual->prox; // Avança para o próximo nó na lista encadeada
        }
    }

    // Verifica se nenhuma área foi encontrada (nenhum livro disponível)
    int nenhumaAreaEncontrada = 1;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (areasProcessadas[i]) {
            nenhumaAreaEncontrada = 0;
            break;
        }
    }

    if (nenhumaAreaEncontrada) {
        printf("Nenhum livro disponível.\n");
    }
}

// Função para listar o livro mais recente
void listarLivroMaisRecente() {
    Livro *livrosMaisRecentes[HASH_TABLE_SIZE];
    int anoMaisRecente = 0;
    int numLivrosMaisRecentes = 0;

    // Encontra o ano mais recente percorrendo toda a tabela hash
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *current = hashTable[i];
        while (current != NULL) {
            Livro *livro = current->livro;

            if (livro->ano_publicacao > anoMaisRecente) {
                // Se encontrou um ano mais recente, limpa os livros anteriores
                anoMaisRecente = livro->ano_publicacao;
                numLivrosMaisRecentes = 0;
                livrosMaisRecentes[numLivrosMaisRecentes++] = livro;
            } else if (livro->ano_publicacao == anoMaisRecente) {
                // Se encontrou outro livro com o mesmo ano mais recente, adiciona à lista
                livrosMaisRecentes[numLivrosMaisRecentes++] = livro;
            }

            current = current->prox;
        }
    }

    // Se encontrou livros mais recentes, imprime suas informações
    if (numLivrosMaisRecentes > 0) {
        printf("Livro(s) mais recente(s) (ano %d):\n", anoMaisRecente);
        for (int i = 0; i < numLivrosMaisRecentes; i++) {
            Livro *livro = livrosMaisRecentes[i];
            printf("ISBN: %s\n", livro->isbn);
            printf("Título: %s\n", livro->titulo);
            printf("Autor: %s\n", livro->autor);
            printf("Área: %s\n", livro->area);
            printf("Ano de Publicação: %d\n", livro->ano_publicacao);
            if (strlen(livro->id_requisitante) > 0) {
                printf("(Requisitado por %s)\n", livro->id_requisitante);
            }
            printf("\n");
        }
    } else {
        printf("Nenhum livro encontrado na biblioteca.\n");
    }
}

// Função para requisitar um livro
int requisitarLivro(const char *isbn, const char *id_requisitante) {
    Livro *livro = pesquisarLivro(isbn);

    if (livro == NULL) {
        printf("Livro com ISBN %s não encontrado.\n", isbn);
        return 0; // Indica falha na requisição
    }

    // Verifica se o livro já está requisitado
    if (strlen(livro->id_requisitante) > 0) {
        printf("Livro com ISBN %s já está requisitado por %s.\n", isbn, livro->id_requisitante);
        return 0; // Indica falha na requisição
    }

    // Atualiza o id_requisitante do livro encontrado
    strncpy(livro->id_requisitante, id_requisitante, sizeof(livro->id_requisitante) - 1);
    livro->id_requisitante[sizeof(livro->id_requisitante) - 1] = '\0'; // Garante terminação nula
    
    // Incrementa o contador de requisições
    livro->contador_requisicoes++; // Incrementa o contador de requisições
    
    printf("Livro com ISBN %s requisitado por %s. Requisitado %d vezes.\n", isbn, livro->id_requisitante, livro->contador_requisicoes);

    return 1; // Indica sucesso na requisição
}

// Função para devolver um livro
int devolverLivro(const char *isbn) {
    Livro *livro = pesquisarLivro(isbn);
    if (livro == NULL) {
        printf("Livro com ISBN %s não encontrado.\n", isbn);
        log_error("Livro não encontrado");
        return 0;
    }
    if (strlen(livro->id_requisitante) == 0) {
        printf("Livro com ISBN %s não está requisitado.\n", isbn);
        log_error("Livro não requisitado");
        return 0;
    }
    livro->id_requisitante[0] = '\0';
    return 1;
}
