#include "locais.h"
#include <stdio.h>
#include <string.h>
#include "biblioteca.h"

int carregarDistritos(Distrito *distritos, int *numDistritos,
                      const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo de distritos");
    return 0; // Retorna 0 para indicar falha
  }
  // Carregar distritos...
  fclose(file);
  return 1; // Retorna 1 para indicar sucesso
}

int carregarConcelhos(Concelho *concelhos, int *numConcelhos,
                      const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo de concelhos");
    return 0; // Retorna 0 para indicar falha
  }
  // Carregar concelhos...
  fclose(file);
  return 1; // Retorna 1 para indicar sucesso
}

int carregarFreguesias(Freguesia *freguesias, int *numFreguesias,
                       const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo de freguesias");
    return 0; // Retorna 0 para indicar falha
  }
  // Carregar freguesias...
  fclose(file);
  return 1; // Retorna 1 para indicar sucesso
}
