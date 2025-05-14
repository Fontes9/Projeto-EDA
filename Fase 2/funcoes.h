/**
 * @file funcoes.h
 * @brief Cabeçalho para gestão de grafos de antenas e interferências
 * @author Tiago Fontes
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINHAS 12
#define MAX_COLUNAS 12
#define MAX_VERTICES 50

typedef struct Vertice {
    char frequencia;
    int coluna;
    int linha;
    int id;
} Vertice;

typedef struct Grafo {
    Vertice vertices[MAX_VERTICES];
    int matriz_adj[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} Grafo;

typedef struct EfeitoNefasto {
    int coluna;
    int linha;
    char frequencia;
    struct EfeitoNefasto *prox;
} nefasto;

Grafo InicializarGrafo();
Grafo CarregarGrafoDoFicheiro(const char* nome_ficheiro);
bool AdicionarVertice(Grafo* grafo, char freq, int coluna, int linha);
void ConectarVerticesMesmaFrequencia(Grafo* grafo);
void BuscaProfundidade(Grafo grafo, int id_origem, bool visitados[]);
void BuscaLargura(Grafo grafo, int id_origem);
void EncontrarTodosCaminhos(Grafo grafo, int origem, int destino, int caminho[], bool visitados[], int pos);
void ListarIntersecoes(Grafo grafo, char freqA, char freqB);
void ImprimirGrafo(Grafo grafo);
int EncontrarIndiceVertice(Grafo grafo, int coluna, int linha);
void ImprimirMatrizAdjacencia(Grafo grafo);
nefasto* CalcularEfeitosNefastosGrafo(Grafo grafo);
void LibertarEfeitosNefastos(nefasto* lista);

#endif