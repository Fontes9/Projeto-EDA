#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_LINHAS 12
#define MAX_COLUNAS 12
#define MAX_ANTENAS 50

typedef struct Adjacencia {
    int destino;  // Índice da antena adjacente
    struct Adjacencia* proxima;
} Adjacencia;

typedef struct Antena {
    char frequencia;
    int coluna;
    int linha;
    Adjacencia* conexoes;
    bool visitada;
} Antena;

typedef struct Grafo {
    Antena antenas[MAX_ANTENAS];
    int total_antenas;
} Grafo;

Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro);
void ProcuraEmProfundidade(Grafo* grafo, int inicio, FILE* saida);
void ProcuraEmLargura(Grafo* grafo, int inicio, FILE* saida);
void EncontrarCaminhos(Grafo* grafo, int origem, int destino, FILE* saida);
void MostrarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida);
void CalcularInterferencias(Grafo* grafo, FILE* saida);
void ExportarResultados(Grafo grafo, const char* nome_ficheiro);
void LibertarGrafo(Grafo* grafo);

#endif