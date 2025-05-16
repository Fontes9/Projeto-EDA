#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINHAS 12
#define MAX_COLUNAS 12
#define MAX_ANTENAS 50

typedef struct Ligacao {
    int id_antena_destino;
    struct Ligacao* proxima;
} Ligacao;

typedef struct Antena {
    char frequencia;
    int coluna;
    int linha;
    Ligacao* conexoes;
    bool visitada;
} Antena;

typedef struct Grafo {
    Antena antenas[MAX_ANTENAS];
    int total_antenas;
} Grafo;

// Carregamento do grafo
Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro);

// Operações do grafo
void ProcuraEmProfundidade(Grafo* grafo, int id_inicio, FILE* saida);
void ProcuraEmLargura(Grafo* grafo, int id_inicio, FILE* saida);
void EncontrarTodosCaminhos(Grafo* grafo, int origem, int destino, FILE* saida);
void ListarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida);
void CalcularEfeitosNefastos(Grafo* grafo, FILE* saida);

// Exportação e libertação de memória
void ExportarResultados(Grafo grafo, const char* nome_ficheiro);
void LibertarGrafo(Grafo* grafo);

#endif