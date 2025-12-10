#ifndef LIVROS_H
#define LIVROS_H

#define HASH_TABLE_SIZE 100

typedef struct Livro {
    char isbn[14];
    char titulo[100];
    char autor[100];
    char area[50];
    int ano_publicacao;
    char id_requisitante[10];
    int contador_requisicoes;
    struct Livro *prox;
} Livro;

void listarLivros();
void removerLivrosNuncaRequisitados();
void listarLivrosRestantes();
void adicionarLivro(Livro novoLivro);
void removerLivro(const char *isbn);
void liberarLivros();
Livro* pesquisarLivro(const char *isbn);
void listarLivrosAgrupadosPorArea();
void listarLivroMaisRecente();
int requisitarLivro(const char *isbn, const char *id_requisitante);
int devolverLivro(const char *isbn);

#endif // LIVROS_H
