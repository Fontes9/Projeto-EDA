#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_LINHAS 12
#define MAX_COLUNAS 12

typedef struct Antena {
    char frequencia;
    int coluna;
    int linha;
    struct Antena *prox;
} antena;

typedef struct EfeitoNefasto {
    int coluna;
    int linha;
    struct EfeitoNefasto *prox;
} nefasto;

// Protótipos das funções
antena* CriarAntena(int coluna, int linha, char freq);
nefasto* CalcularEfeitosNefastos(antena* lista);
antena* CarregarAntenasDoFicheiro(const char* nome_ficheiro);
nefasto* CalcularEfeitosNefastos(antena* lista);
void LibertarAntenas(antena* lista);
void LibertarEfeitosNefastos(nefasto* lista);
void ListarAntenas(antena* lista);
void ListarEfeitosNefastos(nefasto* lista);

#endif