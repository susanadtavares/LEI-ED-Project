#include "biblioteca.h"
#include "requisitantes.h"
#include "areas.h"
#include "livros.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// Função para registrar a mensagem de erro no arquivo logs.txt
void log_error(const char *error_message) {
    FILE *file = fopen("logs.txt", "a"); // Abrir o arquivo em modo de adição
    if (file == NULL) {
        // Se não for possível abrir o arquivo, exibir um erro na saída padrão de erro
        fprintf(stderr, "Não foi possível abrir o arquivo de log: %s\n", strerror(errno));
        return;
    }

    // Obter a hora atual
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Formatar a hora como string
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    // Escrever a mensagem de erro no arquivo com a hora
    fprintf(file, "%s - erro: %s\n", time_str, error_message);
    fclose(file); // Fechar o arquivo
}

void inicializarBiblioteca(Biblioteca *biblioteca) {
    biblioteca->livros = NULL;
    biblioteca->requisitantes = NULL;
    biblioteca->areas = NULL;
    biblioteca->numDistritos = 0;
    biblioteca->numConcelhos = 0;
    biblioteca->numFreguesias = 0;
}

void carregarDadosBiblioteca(Biblioteca *biblioteca) {
    printf("A carregar distritos...\n");
    if (!carregarDistritos(biblioteca->distritos, &biblioteca->numDistritos, "./txt/distritos.txt")) {
        fprintf(stderr, "Erro ao carregar distritos\n");
        log_error("Erro ao carregar distritos");
        return;
    }
    printf("A carregar concelhos...\n");
    if (!carregarConcelhos(biblioteca->concelhos, &biblioteca->numConcelhos, "./txt/concelhos.txt")) {
        fprintf(stderr, "Erro ao carregar concelhos\n");
        log_error("Erro ao carregar concelhos");
        return;
    }
    printf("A carregar freguesias...\n");
    if (!carregarFreguesias(biblioteca->freguesias, &biblioteca->numFreguesias, "./txt/freguesias.txt")) {
        fprintf(stderr, "Erro ao carregar freguesias\n");
        log_error("Erro ao carregar freguesias");
        return;
    }
    printf("A carregar requisitantes...\n");
    biblioteca->requisitantes = carregarRequisitantes("./txt/requisitantes.txt");
    if (!biblioteca->requisitantes) {
        fprintf(stderr, "Erro ao carregar requisitantes\n");
        log_error("Erro ao carregar requisitantes");
    }
}

void salvarDadosBiblioteca(Biblioteca *biblioteca) {
    salvarRequisitantes(biblioteca->requisitantes, "./txt/requisitantes.txt");
}

// Guardar dados biblioteca
void GuardarBiblioteca(const char *nomeArquivo, Biblioteca *bib) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }

    // Salvar a estrutura principal da Biblioteca
    fwrite(bib, sizeof(Biblioteca), 1, arquivo);

    // Salvar livros
    Livro *livroAtual = bib->livros;
    while (livroAtual != NULL) {
        fwrite(livroAtual, sizeof(Livro), 1, arquivo);
        livroAtual = livroAtual->prox;
    }
    // Escrever um indicador de fim de lista para livros
    Livro livroFim = {"", "", "", "", 0, "", 0, NULL};
    fwrite(&livroFim, sizeof(Livro), 1, arquivo);

    // Salvar requisitantes
    Requisitante *requisitanteAtual = bib->requisitantes;
    while (requisitanteAtual != NULL) {
        fwrite(requisitanteAtual, sizeof(Requisitante), 1, arquivo);
        requisitanteAtual = requisitanteAtual->prox;
    }
    // Escrever um indicador de fim de lista para requisitantes
    Requisitante requisitanteFim = {"", "", "", "", NULL};
    fwrite(&requisitanteFim, sizeof(Requisitante), 1, arquivo);

    // Salvar áreas
    Area *areaAtual = bib->areas;
    while (areaAtual != NULL) {
        fwrite(areaAtual, sizeof(Area), 1, arquivo);
        areaAtual = areaAtual->proxima;
    }
    // Escrever um indicador de fim de lista para áreas
    Area areaFim = {"", NULL};
    fwrite(&areaFim, sizeof(Area), 1, arquivo);

    fclose(arquivo);
}

// Guardar dados livros
void GuardarLivros(const char *nomeArquivo, Livro *livros) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }

    Livro *livroAtual = livros;
    while (livroAtual != NULL) {
        fwrite(livroAtual, sizeof(Livro), 1, arquivo);
        livroAtual = livroAtual->prox;
    }
    // Escrever um indicador de fim de lista para livros
    Livro livroFim = {"", "", "", "", 0, "", 0, NULL};
    fwrite(&livroFim, sizeof(Livro), 1, arquivo);

    fclose(arquivo);
}

// Guardar dados requisitantes
void GuardarRequisitantes(const char *nomeArquivo, Requisitante *requisitantes) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }

    Requisitante *requisitanteAtual = requisitantes;
    while (requisitanteAtual != NULL) {
        fwrite(requisitanteAtual, sizeof(Requisitante), 1, arquivo);
        requisitanteAtual = requisitanteAtual->prox;
    }
    // Escrever um indicador de fim de lista para requisitantes
    Requisitante requisitanteFim = {"", "", "", "", NULL};
    fwrite(&requisitanteFim, sizeof(Requisitante), 1, arquivo);

    fclose(arquivo);
}

// Guardar dados areas
void GuardarAreas(const char *nomeArquivo, Area *areas) {
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }

    Area *areaAtual = areas;
    while (areaAtual != NULL) {
        fwrite(areaAtual, sizeof(Area), 1, arquivo);
        areaAtual = areaAtual->proxima;
    }
    // Escrever um indicador de fim de lista para áreas
    Area areaFim = {"", NULL};
    fwrite(&areaFim, sizeof(Area), 1, arquivo);

    fclose(arquivo);
}

void CarregarBiblioteca(const char *nomeArquivo, Biblioteca *bib) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }

    // Carregar a estrutura principal da Biblioteca
    fread(bib, sizeof(Biblioteca), 1, arquivo);

    // Carregar livros
    Livro *livroAnterior = NULL;
    Livro livroTemp;
    bib->livros = NULL;
    while (fread(&livroTemp, sizeof(Livro), 1, arquivo) == 1) {
        if (livroTemp.prox == NULL) break; // Indicador de fim de lista
        Livro *novoLivro = (Livro *)malloc(sizeof(Livro));
        *novoLivro = livroTemp;
        novoLivro->prox = NULL;
        if (livroAnterior == NULL) {
            bib->livros = novoLivro;
        } else {
            livroAnterior->prox = novoLivro;
        }
        livroAnterior = novoLivro;
    }

    // Carregar requisitantes
    Requisitante *requisitanteAnterior = NULL;
    Requisitante requisitanteTemp;
    bib->requisitantes = NULL;
    while (fread(&requisitanteTemp, sizeof(Requisitante), 1, arquivo) == 1) {
        if (requisitanteTemp.prox == NULL) break; // Indicador de fim de lista
        Requisitante *novoRequisitante = (Requisitante *)malloc(sizeof(Requisitante));
        *novoRequisitante = requisitanteTemp;
        novoRequisitante->prox = NULL;
        if (requisitanteAnterior == NULL) {
            bib->requisitantes = novoRequisitante;
        } else {
            requisitanteAnterior->prox = novoRequisitante;
        }
        requisitanteAnterior = novoRequisitante;
    }

    // Carregar áreas
    Area *areaAnterior = NULL;
    Area areaTemp;
    bib->areas = NULL;
    while (fread(&areaTemp, sizeof(Area), 1, arquivo) == 1) {
        if (areaTemp.proxima == NULL) break; // Indicador de fim de lista
        Area *novaArea = (Area *)malloc(sizeof(Area));
        *novaArea = areaTemp;
        novaArea->proxima = NULL;
        if (areaAnterior == NULL) {
            bib->areas = novaArea;
        } else {
            areaAnterior->proxima = novaArea;
        }
        areaAnterior = novaArea;
    }

    fclose(arquivo);
}

void Carregarlivros(const char *nomeArquivo, Livro *liv) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }
    fread(liv, sizeof(Livro), 1, arquivo);
    fclose(arquivo);
}

void CarregarRequisitantes(const char *nomeArquivo, Requisitante *rq) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }
    fread(rq, sizeof(Requisitante), 1, arquivo);
    fclose(arquivo);
}

void CarregarAreas(const char *nomeArquivo, Area *area) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        log_error("Erro ao abrir o arquivo");
        return;
    }
    fread(area, sizeof(Area), 1, arquivo);
    fclose(arquivo);
}

void liberarBiblioteca(Biblioteca *biblioteca) {
    liberarLivros(biblioteca->livros);
    liberarRequisitantes(biblioteca->requisitantes);

    Area *atual = biblioteca->areas;
    while (atual != NULL) {
        Area *proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
}

size_t calcularMemoriaBiblioteca(Biblioteca *biblioteca) {
    size_t memoria = sizeof(Biblioteca);  // Memória base da estrutura Biblioteca

    // Calcular memória para livros
    Livro *livro = biblioteca->livros;
    while (livro != NULL) {
        memoria += sizeof(Livro);
        livro = livro->prox;
    }

    // Calcular memória para requisitantes
    Requisitante *requisitante = biblioteca->requisitantes;
    while (requisitante != NULL) {
        memoria += sizeof(Requisitante);
        requisitante = requisitante->prox;
    }

    // Calcular memória para áreas
    Area *area = biblioteca->areas;
    while (area != NULL) {
        memoria += sizeof(Area);
        area = area->proxima;
    }

    return memoria;
}

void submenuLivros(Biblioteca *biblioteca) {
    Livro *livros = NULL;
    int opcao;
    char isbn[14];
    do {
        printf("\n--- Submenu Livros ---\n");
        printf("1. Listar Livros\n");
        printf("2. Adicionar Livro\n");
        printf("3. Remover Livro\n");
        printf("4. Pesquisar Livro\n");
        printf("5. Listar Livros por Área\n");
        printf("6. Remover Livros Nunca Requisitados e Listar Livros Restantes\n");
        printf("7. Livro mais recente\n");
        printf("8. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            log_error("Entrada inválida");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }
        getchar();  // Limpar o caractere de nova linha deixado pelo scanf

        switch (opcao) {
            case 1:
                listarLivros();
                break;
            case 2: {
                Livro novoLivro;
                printf("ISBN: ");
                fgets(novoLivro.isbn, sizeof(novoLivro.isbn), stdin);
                strtok(novoLivro.isbn, "\n");

                printf("Título: ");
                fgets(novoLivro.titulo, sizeof(novoLivro.titulo), stdin);
                strtok(novoLivro.titulo, "\n");
                printf("Autor: ");
                fgets(novoLivro.autor, sizeof(novoLivro.autor), stdin);
                strtok(novoLivro.autor, "\n");
                printf("Área: ");
                fgets(novoLivro.area, sizeof(novoLivro.area), stdin);
                strtok(novoLivro.area, "\n");

                // Verificar se a área existe
                if (!areaExiste(biblioteca->areas, novoLivro.area)) {
                    printf("Área não existe! Por favor, adicione a área primeiro.\n");
                    log_error("Área não existe");
                    break;
                }

                printf("Ano de Publicação: ");
                if (scanf("%d", &novoLivro.ano_publicacao) != 1) {
                    printf("Ano de publicação inválido!\n");
                    log_error("Ano de publicação inválido");
                    while (getchar() != '\n'); // Limpar o buffer de entrada
                    continue;
                }
                getchar();
                adicionarLivro(novoLivro);
                break;
            }
            case 3:
                printf("Digite o ISBN do livro a ser removido: ");
                fgets(isbn, sizeof(isbn), stdin);
                strtok(isbn, "\n");

                removerLivro(isbn);
                break;
            case 4:
                printf("Digite o ISBN do livro a ser pesquisado: ");
                fgets(isbn, sizeof(isbn), stdin);
                strtok(isbn, "\n");

                Livro *livroEncontrado = pesquisarLivro(isbn);
                if (livroEncontrado != NULL) {
                    printf("\n--- Livro Encontrado ---\n");
                    printf("ISBN: %s\n", livroEncontrado->isbn);
                    printf("Título: %s\n", livroEncontrado->titulo);
                    printf("Autor: %s\n", livroEncontrado->autor);
                    printf("Área: %s\n", livroEncontrado->area);
                    printf("Ano de Publicação: %d\n", livroEncontrado->ano_publicacao);
                    if (strlen(livroEncontrado->id_requisitante) > 0) {
                        printf("(Requisitado por %s)\n", livroEncontrado->id_requisitante);
                    }
                } else {
                    printf("Livro com ISBN '%s' não encontrado.\n", isbn);
                    log_error("Livro nao encontrado");
                }
                break;
            case 5:
                listarLivrosAgrupadosPorArea(biblioteca->livros);
                break;
            case 6:
                removerLivrosNuncaRequisitados();
                break;
            case 7:
                listarLivroMaisRecente(biblioteca->livros);
            case 8:
                return;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                log_error("Opcao invalida");
        }
    } while (opcao != 6);
}

void submenuRequisicoes(Biblioteca *biblioteca) {
    int opcao;
    char isbn[14];
    char id_requisitante[10];
    do {
        printf("\n--- Submenu Requisições ---\n");
        printf("1. Listar Livros\n");
        printf("2. Adicionar requisição\n");
        printf("3. Devolver Livro\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            log_error("Entrada invalida");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }
        getchar();  // Limpar o caractere de nova linha deixado pelo scanf

        switch (opcao) {
            case 1:
                listarLivrosRestantes(biblioteca->livros);
                break;
            case 2: {
                printf("ISBN do livro a ser requisitado: ");
                fgets(isbn, sizeof(isbn), stdin);
                strtok(isbn, "\n");
                printf("ID do requisitante: ");
                fgets(id_requisitante, sizeof(id_requisitante), stdin);
                strtok(id_requisitante, "\n");
                if (requisitarLivro(isbn, id_requisitante)) {
                    printf("Livro requisitado com sucesso.\n");
                } else {
                    printf("Falha ao requisitar o livro.\n");
                    log_error("Falha ao requisitar o livro");
                }
                break;
            }
            case 3:
                printf("ISBN do livro a ser devolvido: ");
                fgets(isbn, sizeof(isbn), stdin);
                strtok(isbn, "\n");
                if (devolverLivro(isbn)) {
                    printf("Livro devolvido com sucesso.\n");
                } else {
                    printf("Falha ao devolver o livro.\n");
                    log_error("Falha ao devolver o livro");
                }
                break;
            case 4:
                return;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                log_error("Opcao invalida");
        }
    } while (opcao != 6);
}

void submenuRequisitantes(Biblioteca *biblioteca) {
    int opcao;
    char id[10];
    char nome[100];
    
    do {
        printf("\n--- Submenu Requisitantes ---\n");
        printf("1. Listar Requisitantes\n");
        printf("2. Adicionar Requisitante\n");
        printf("3. Remover Requisitante\n");
        printf("4. Pesquisar Requisitante\n");
        printf("5. Listar Requisitantes (A-Z)\n");
        printf("6. Listar Requisitantes por ID de Freguesia\n");
        printf("7. Listar Requisitantes (A-Z Apelido)\n");
        printf("8. Mostrar Estatísticas\n");
        printf("9. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            log_error("Entrada invalida");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }
        getchar();  // Limpar o caractere de nova linha deixado pelo scanf

        switch (opcao) {
            case 1:
                listarRequisitantes(biblioteca->requisitantes);
                break;
            case 2:
                adicionarRequisitante(&biblioteca->requisitantes);
                break;
            case 3:
                printf("ID do requisitante a ser removido: ");
                fgets(id, sizeof(id), stdin);
                strtok(id, "\n");
                removerRequisitante(&biblioteca->requisitantes, id);
                break;
            case 4:
                printf("Nome do requisitante: ");
                fgets(nome, sizeof(nome), stdin);
                strtok(nome, "\n");
                pesquisarRequisitante(biblioteca->requisitantes, nome);
                break;
            case 5: {
                Requisitante *listaOrdenada = ordenarPorNome(biblioteca->requisitantes);
                listarRequisitantes(listaOrdenada);
                liberarRequisitantes(listaOrdenada);
                break;
            }
            case 6: {
                Requisitante *listaOrdenada = ordenarPorIdFreguesia(biblioteca->requisitantes);
                listarRequisitantes(listaOrdenada);
                liberarRequisitantes(listaOrdenada);
                break;
            }
            case 7: {
                Requisitante *listaOrdenada = ordenarPorApelido(biblioteca->requisitantes);
                listarRequisitantes(listaOrdenada);
                liberarRequisitantes(listaOrdenada);
                break;
            }
            case 8:
                mostrarEstatisticas(biblioteca->requisitantes);
                printf("\n===[ Idade com mais requisitantes ]===\n");
                listarIdadeMaisRequisitantes(biblioteca->requisitantes);
                printf("\n===[ Media de idades de todos os requisitantes ]===\n");
                mediaIdadesRequisitantes(biblioteca->requisitantes);
                printf("\n===[ Ano de nascimento com mais requisitantes ]===\n");
                idadeMaximaRequisitantes(biblioteca->requisitantes);
                break;
            case 9:
                return;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                log_error("Opcao invalida");
        }
    } while (opcao != 8);
}

void submenuAreas(Biblioteca *biblioteca) {
    int opcao;
    char nome[100];
    do {
        printf("\n--- Submenu Áreas ---\n");
        printf("1. Listar Áreas\n");
        printf("2. Adicionar Área\n");
        printf("3. Remover Área\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            log_error("Entrada invalida");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }
        getchar();  // Limpar o caractere de nova linha deixado pelo scanf

        switch (opcao) {
            case 1:
                listarAreas(biblioteca->areas);
                break;
            case 2: {
                char nomeArea[100];
                printf("Nome da nova área: ");
                if (fgets(nomeArea, sizeof(nomeArea), stdin) != NULL) {
                    nomeArea[strcspn(nomeArea, "\n")] = '\0';
                    if (adicionarArea(&biblioteca->areas, nomeArea)) {
                        printf("Área adicionada com sucesso.\n");
                    } else {
                        printf("Erro ao adicionar a área.\n");
                        log_error("Erro ao adicionar a área");
                    }
                } else {
                    fprintf(stderr, "Erro ao ler a nova área.\n");
                    log_error("Erro ao ler a nova área");
                }
                break;
            }
            case 3:
                printf("\nNome da área a ser removida: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                if (removerArea(&biblioteca->areas, nome)) {
                    printf("Área '%s' removida com sucesso.\n", nome);
                } else {
                    printf("Erro ao remover a área '%s'.\n", nome);
                    log_error("Erro ao remover a área");
                }
                break;
            case 4:
                return;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                log_error("Opcao invalida");
        }
    } while (opcao != 3);
}

void menu(Biblioteca *biblioteca) {
    Livro livro;
    Requisitante requisitante;
    Area area;
    int opcao;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Gestão de Livros\n");
        printf("2. Gestão de Requisitantes\n");
        printf("3. Gestão de Requisições\n");
        printf("4. Gestão de Áreas\n");
        printf("5. Calcular Memória\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Por favor, insira um número.\n");
            log_error("Entrada invalida");
            while (getchar() != '\n'); // Limpar o buffer de entrada
            continue;
        }
        getchar();  // Limpar o caractere de nova linha deixado pelo scanf

        switch (opcao) {
            case 1:
                submenuLivros(biblioteca);
                break;
            case 2:
                submenuRequisitantes(biblioteca);
                break;
            case 3:
                submenuRequisicoes(biblioteca);
                break;
            case 4:
                submenuAreas(biblioteca);
                break;
            case 5:
                printf("Memória ocupada pela biblioteca: %zu bytes\n", calcularMemoriaBiblioteca(biblioteca));
                break;
            case 6:
                GuardarBiblioteca("biblioteca.bin", biblioteca);
                GuardarLivros("livros.bin", biblioteca->livros);
                GuardarRequisitantes("requisitantes.bin", biblioteca->requisitantes);
                GuardarAreas("areas.bin", biblioteca->areas);
                printf("A encerrar o programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                log_error("Opcao invalida");
        }
    } while (opcao != 6);
}