#include "requisitantes.h"
#include "biblioteca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#define ID_SIZE 10
#define FILENAME "txt/requisitantes.txt"

// Definição de datas para a quaresma em 2024 (estas datas variam a cada ano)
#define CARNAVAL "13-02-2024"
#define PASCOA "31-03-2024"

// Função para carregar requisitantes do arquivo
Requisitante *carregarRequisitantes(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de requisitantes");
        return NULL;
    }
    Requisitante *lista = NULL, novoRequisitante;
    while (fscanf(file, "%9s\t%99[^\t]\t%10s\t%6s\n",
                  novoRequisitante.id_requisitante, novoRequisitante.nome,
                  novoRequisitante.data_nasc, novoRequisitante.id_freguesia) == 4) {
        Requisitante *novo = (Requisitante *)malloc(sizeof(Requisitante));
        if (novo) {
            *novo = novoRequisitante;
            novo->prox = lista;
            lista = novo;
        }
    }
    fclose(file);
    return lista;
}

// Função para salvar requisitantes no arquivo
void salvarRequisitantes(Requisitante *lista, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao abrir o arquivo de requisitantes para salvar");
        return;
    }
    Requisitante *atual = lista;
    while (atual != NULL) {
        fprintf(file, "%s\t%s\t%s\t%s\n",
                atual->id_requisitante, atual->nome,
                atual->data_nasc, atual->id_freguesia);
        atual = atual->prox;
    }
    fclose(file);
}

// Função para liberar a memória dos requisitantes
void liberarRequisitantes(Requisitante *lista) {
    Requisitante *atual = lista;
    while (atual != NULL) {
        Requisitante *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

// Função para listar os requisitantes
void listarRequisitantes(Requisitante *lista) {
    Requisitante *atual = lista;
    while (atual != NULL) {
        char nomeMaiusculo[100];
        strncpy(nomeMaiusculo, atual->nome, sizeof(nomeMaiusculo) - 1);
        nomeMaiusculo[sizeof(nomeMaiusculo) - 1] = '\0';  
        toUpperCase(nomeMaiusculo);
        printf("ID: %s, Nome: %s, Data de Nascimento: %s, Freguesia: %s\n",
               atual->id_requisitante, nomeMaiusculo, atual->data_nasc,
               atual->id_freguesia);
        atual = atual->prox;
    }
}

// Função para verificar se um ID já existe no arquivo
int idExiste(const char *id) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo de requisitantes");
        exit(EXIT_FAILURE);
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        char id_requisitante[ID_SIZE];
        sscanf(linha, "%9s", id_requisitante);
        if (strcmp(id, id_requisitante) == 0) {
            fclose(file);
            return 1; // ID encontrado
        }
    }

    fclose(file);
    return 0; // ID não encontrado
}

// Função para gerar um ID único para o requisitante
int gerarIdRequisitante(char *id_requisitante) {
    int sum = 0;
    for (int i = 0; i < ID_SIZE - 2; i++) {
        int digit = rand() % 10; // Gera um dígito aleatório de 0 a 9
        id_requisitante[i] = '0' + digit; // Converte o dígito para char e armazena no ID
        sum += digit; // Adiciona o dígito à soma
    }

    // Calcula o dígito de controle para que a soma seja múltiplo de 10
    int controlDigit = (10 - (sum % 10)) % 10;
    id_requisitante[ID_SIZE - 2] = '0' + controlDigit; // Define o dígito de controle

    id_requisitante[ID_SIZE - 1] = '\0'; // Termina a string

    // Verifica se o ID gerado é único
    return !idExiste(id_requisitante);
}

// Função para adicionar um novo requisitante ao final da lista
void adicionarRequisitante(Requisitante **lista) {
    srand(time(0));
    Requisitante novoRequisitante;
    if (!gerarIdRequisitante(novoRequisitante.id_requisitante)) {
        printf("Erro ao gerar ID de requisitante.\n");
        return;
    }
    printf("Nome: ");
    fgets(novoRequisitante.nome, sizeof(novoRequisitante.nome), stdin);
    strtok(novoRequisitante.nome, "\n");
    printf("Data de Nascimento (DD-MM-YYYY): ");
    fgets(novoRequisitante.data_nasc, sizeof(novoRequisitante.data_nasc), stdin);
    strtok(novoRequisitante.data_nasc, "\n");
    while ((getchar()) != '\n');
    printf("ID da Freguesia: ");
    fgets(novoRequisitante.id_freguesia, sizeof(novoRequisitante.id_freguesia), stdin);
    strtok(novoRequisitante.id_freguesia, "\n");
    while ((getchar()) != '\n');
    Requisitante *novo = (Requisitante *)malloc(sizeof(Requisitante));
    if (novo) {
        *novo = novoRequisitante;
        novo->prox = NULL;

        if (*lista == NULL) {
            *lista = novo;
        } else {
            Requisitante *atual = *lista;
            while (atual->prox != NULL) {
                atual = atual->prox;
            }
            atual->prox = novo;
        }
    }
    // Salva a lista atualizada no arquivo
    salvarRequisitantes(*lista, FILENAME);
    printf("Requisitante Adicionado com sucesso!\n");
}

// Função para remover um requisitante pelo ID
void removerRequisitante(Requisitante **lista, const char *id) {
    Requisitante *atual = *lista, *anterior = NULL;

    while (atual != NULL && strcmp(atual->id_requisitante, id) != 0) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Requisitante com ID %s não encontrado.\n", id);
        return;
    }

    if (anterior == NULL) {
        *lista = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    salvarRequisitantes(*lista, FILENAME);
    printf("Requisitante removido com sucesso!\n");
}

// Função para comparar strings de forma insensível a maiúsculas e minúsculas
int compararStringsInsensivel(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

// Função para pesquisar requisitante pelo nome
void pesquisarRequisitante(Requisitante *lista, const char *nome) {
    Requisitante *atual = lista;
    int encontrado = 0;
    while (atual != NULL) {
        if (compararStringsInsensivel(atual->nome, nome)) {
            printf("ID: %s, Nome: %s, Data de Nascimento: %s, Freguesia: %s\n",
                   atual->id_requisitante, atual->nome, atual->data_nasc,
                   atual->id_freguesia);
            encontrado = 1;
        }
        atual = atual->prox;
    }

    if (!encontrado) {
        printf("Nenhum requisitante encontrado com o nome '%s'.\n", nome);
    }
}

Requisitante* copiarListaRequisitantes(Requisitante *requisitantes) {
    if (requisitantes == NULL) return NULL;

    Requisitante *copia = malloc(sizeof(Requisitante));
    if (copia == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        log_error("Erro de alocação de memória");
        exit(1);
    }
    *copia = *requisitantes;
    copia->prox = copiarListaRequisitantes(requisitantes->prox);

    return copia;
}

void liberarLista(Requisitante *requisitantes) {
    while (requisitantes != NULL) {
        Requisitante *temp = requisitantes;
        requisitantes = requisitantes->prox;
        free(temp);
    }
}

Requisitante* ordenarPorNome(Requisitante *requisitantes) {
    if (requisitantes == NULL || requisitantes->prox == NULL) return copiarListaRequisitantes(requisitantes);

    Requisitante *copia = copiarListaRequisitantes(requisitantes);
    // Implementação do algoritmo de ordenação (por exemplo, bubble sort)
    // Ordenação por nome
    for (Requisitante *i = copia; i != NULL; i = i->prox) {
        for (Requisitante *j = i->prox; j != NULL; j = j->prox) {
            if (strcmp(i->nome, j->nome) > 0) {
                // Trocar os dados dos nós i e j
                Requisitante temp = *i;
                *i = *j;
                *j = temp;
                Requisitante *proxTemp = i->prox;
                i->prox = j->prox;
                j->prox = proxTemp;
            }
        }
    }
    return copia;
}

Requisitante* ordenarPorIdFreguesia(Requisitante *requisitantes) {
    if (requisitantes == NULL || requisitantes->prox == NULL) return copiarListaRequisitantes(requisitantes);

    Requisitante *copia = copiarListaRequisitantes(requisitantes);
    // Implementação do algoritmo de ordenação (por exemplo, bubble sort)
    // Ordenação por ID de freguesia
    for (Requisitante *i = copia; i != NULL; i = i->prox) {
        for (Requisitante *j = i->prox; j != NULL; j = j->prox) {
            if (strcmp(i->id_freguesia, j->id_freguesia) > 0) {
                // Trocar os dados dos nós i e j
                Requisitante temp = *i;
                *i = *j;
                *j = temp;
                Requisitante *proxTemp = i->prox;
                i->prox = j->prox;
                j->prox = proxTemp;
            }
        }
    }
    return copia;
}

// Função para extrair o último nome
const char* extrairUltimoNome(const char *nome) {
    const char *ultimoNome = strrchr(nome, ' ');
    return (ultimoNome != NULL) ? ultimoNome + 1 : nome;
}

// Função auxiliar para comparar apelidos (sobrenomes)
int compararApelidos(const void *a, const void *b) {
    const Requisitante *requisitanteA = *(const Requisitante **)a;
    const Requisitante *requisitanteB = *(const Requisitante **)b;
    return strcmp(requisitanteA->nome, requisitanteB->nome);
}

// Função para ordenar os requisitantes por apelido
Requisitante* ordenarPorApelido(Requisitante *requisitantes) {
    // Copiar a lista original de requisitantes
    Requisitante *copia = NULL;
    Requisitante *ultimoCopia = NULL;
    Requisitante *atual = requisitantes;

    while (atual != NULL) {
        Requisitante *novo = (Requisitante *)malloc(sizeof(Requisitante));
        if (!novo) {
            // Tratar erro de alocação de memória
            perror("Erro ao alocar memória para cópia da lista");
            log_error("Erro ao alocar memória para cópia da lista");
            // Se houve algum erro, liberar a memória alocada para a cópia parcial
            while (copia != NULL) {
                Requisitante *temp = copia;
                copia = copia->prox;
                free(temp);
            }
            return NULL;
        }
        *novo = *atual;
        novo->prox = NULL;

        if (copia == NULL) {
            copia = novo;
            ultimoCopia = copia;
        } else {
            ultimoCopia->prox = novo;
            ultimoCopia = novo;
        }

        atual = atual->prox;
    }

    // Ordenar a lista copiada por apelido
    copia = ordenarListaPorApelido(copia);

    return copia;
}

// Função para ordenar uma lista de requisitantes por apelido
Requisitante* ordenarListaPorApelido(Requisitante *requisitantes) {
    // Conta o número de requisitantes
    int tamanho = 0;
    Requisitante *atual = requisitantes;
    while (atual != NULL) {
        tamanho++;
        atual = atual->prox;
    }

    // Cria um array de ponteiros para requisitantes
    Requisitante **arrayRequisitantes = (Requisitante **)malloc(tamanho * sizeof(Requisitante *));
    if (!arrayRequisitantes) {
        // Tratar erro de alocação de memória
        perror("Erro ao alocar memória para ordenação");
        log_error("Erro ao alocar memória para ordenação");
        return NULL;
    }

    // Preenche o array de ponteiros
    int i = 0;
    atual = requisitantes;
    while (atual != NULL) {
        arrayRequisitantes[i] = atual;
        atual = atual->prox;
        i++;
    }

    // Ordena o array de ponteiros usando a função de comparação
    qsort(arrayRequisitantes, tamanho, sizeof(Requisitante *), compararApelidos);

    // Reconstrói a lista ligada ordenada
    Requisitante *listaOrdenada = NULL;
    for (int j = tamanho - 1; j >= 0; j--) {
        arrayRequisitantes[j]->prox = listaOrdenada;
        listaOrdenada = arrayRequisitantes[j];
    }

    // Libera o array de ponteiros
    free(arrayRequisitantes);

    return listaOrdenada;
}

// Função auxiliar para calcular a frequência de strings em uma lista de requisitantes
void calcularFrequencia(Requisitante *lista, char *(*extrairFuncao)(Requisitante *), char *maisComum, int *maxFreq, float *percentagem) {
    typedef struct {
        char chave[100];
        int frequencia;
    } Frequencia;

    Frequencia *frequencias = NULL;
    int count = 0;
    int total = 0;

    Requisitante *iter = lista;
    while (iter != NULL) {
        total++;
        iter = iter->prox;
    }

    while (lista != NULL) {
        char *chave = extrairFuncao(lista);
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(frequencias[i].chave, chave) == 0) {
                frequencias[i].frequencia++;
                found = 1;
                break;
            }
        }
        if (!found) {
            frequencias = realloc(frequencias, (count + 1) * sizeof(Frequencia));
            strcpy(frequencias[count].chave, chave);
            frequencias[count].frequencia = 1;
            count++;
        }
        lista = lista->prox;
    }

    *maxFreq = 0;
    for (int i = 0; i < count; i++) {
        if (frequencias[i].frequencia > *maxFreq) {
            *maxFreq = frequencias[i].frequencia;
            strcpy(maisComum, frequencias[i].chave);
        }
    }

    *percentagem = ((float)(*maxFreq) / total) * 100.0f;

    free(frequencias);
}

char *extrairNome(Requisitante *requisitante) {
    return requisitante->nome;
}

char *extrairPrimeiroNome(Requisitante *requisitante) {
    static char primeiroNome[100];
    char *nome = requisitante->nome;
    sscanf(nome, "%s", primeiroNome); // Extrai o primeiro nome até o primeiro espaço
    return primeiroNome;
}

char *extrairSobrenome(Requisitante *requisitante) {
    char *nome = requisitante->nome;
    char *sobrenome = strrchr(nome, ' ');
    return sobrenome ? sobrenome + 1 : nome;
}

char *extrairFreguesia(Requisitante *requisitante) {
    return requisitante->id_freguesia;
}

void mostrarEstatisticas(Requisitante *lista) {
    char maisComum[100];
    int maxFreq;
    float percentagem;

    
    calcularFrequencia(lista, extrairPrimeiroNome, maisComum, &maxFreq, &percentagem);
    printf("Nome mais comum: %s (aparece %d vezes, %.2f%%)\n", maisComum, maxFreq, percentagem);

    calcularFrequencia(lista, extrairSobrenome, maisComum, &maxFreq, &percentagem);
    printf("Sobrenome mais comum: %s (aparece %d vezes, %.2f%%)\n", maisComum, maxFreq, percentagem);

    calcularFrequencia(lista, extrairFreguesia, maisComum, &maxFreq, &percentagem);
    printf("Freguesia mais comum: %s (aparece %d vezes, %.2f%%)\n", maisComum, maxFreq, percentagem);
}

// Função para determinar a idade de um requisitante a partir da data de nascimento
int calcularIdade(const char *data_nasc) {
    int dia, mes, ano;
    sscanf(data_nasc, "%d-%d-%d", &dia, &mes, &ano);

    time_t t = time(NULL);
    struct tm tm_atual = *localtime(&t);

    int idade = tm_atual.tm_year + 1900 - ano;

    if (tm_atual.tm_mon + 1 < mes || (tm_atual.tm_mon + 1 == mes && tm_atual.tm_mday < dia)) {
        idade--;
    }

    return idade;
}

// Função para listar a idade com mais requisitantes
void listarIdadeMaisRequisitantes(Requisitante *lista) {
    int idades[150] = {0}; // Supondo idades entre 0 e 150 anos
    Requisitante *atual = lista;
    while (atual != NULL) {
        int idade = calcularIdade(atual->data_nasc);
        idades[idade]++;
        atual = atual->prox;
    }

    int maxIdade = 0;
    for (int i = 1; i < 150; i++) {
        if (idades[i] > idades[maxIdade]) {
            maxIdade = i;
        }
    }

    printf("Idade com mais requisitantes: %d anos\n", maxIdade);
}

// Função para determinar a média de idades de todos os requisitantes
void mediaIdadesRequisitantes(Requisitante *lista) {
    int totalIdade = 0, count = 0;
    Requisitante *atual = lista;
    while (atual != NULL) {
        totalIdade += calcularIdade(atual->data_nasc);
        count++;
        atual = atual->prox;
    }

    if (count == 0) {
        printf("Nenhum requisitante encontrado.\n");
    } else {
        printf("Média de idades dos requisitantes: %.2f anos\n", (double)totalIdade / count);
    }
}

// Função para determinar a idade máxima de todos os requisitantes
void idadeMaximaRequisitantes(Requisitante *lista) {
    int maxIdade = 0;
    Requisitante *atual = lista;
    while (atual != NULL) {
        int idade = calcularIdade(atual->data_nasc);
        if (idade > maxIdade) {
            maxIdade = idade;
        }
        atual = atual->prox;
    }

    printf("Ano de nascimento com mais requisitantes: %d\n", maxIdade);
}

// Função para converter uma string para maiúsculas
void toUpperCase(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}
