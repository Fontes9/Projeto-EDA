/**
 * @file funcoes.h
 * @brief Definição das estruturas e funções para manipulação do grafo de antenas
 */

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

/**
 * @brief Estrutura que representa uma adjacência
 */

typedef struct Adjacencia {
    int destino;
    struct Adjacencia* proxima;
} Adjacencia;

/**
 * @brief Estrutura que representa uma antena
 */

typedef struct Antena {
    char frequencia;
    int coluna;
    int linha;
    Adjacencia* conexoes;
    bool visitada;
} Antena;

/**
 * @brief Estrutura principal do grafo de antenas
 */

typedef struct Grafo {
    Antena antenas[MAX_ANTENAS];
    int total_antenas;
} Grafo;

/**
 * @brief Carrega as antenas a partir de um ficheiro de texto
 * @param nome_ficheiro Nome do ficheiro contendo os dados das antenas
 * @return Grafo populado com as antenas e suas ligações
 */

Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro);

/**
 * @brief Realiza uma busca em profundidade a partir de uma antena
 * @param grafo Apontador para o grafo de antenas
 * @param inicio Índice da antena inicial para a busca
 * @param saida ficheiro de saída para gravar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool ProcuraEmProfundidade(Grafo* grafo, int inicio, FILE* saida);

/**
 * @brief Realiza uma busca em largura a partir de uma antena
 * @param grafo Apontador para o grafo de antenas
 * @param inicio Índice da antena inicial para a busca
 * @param saida ficheiro de saída para gravar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool ProcuraEmLargura(Grafo* grafo, int inicio, FILE* saida);
/**
 * @brief Encontra todos os caminhos entre duas antenas
 * @param grafo Apontador para o grafo de antenas
 * @param origem Índice da antena de origem
 * @param destino Índice da antena de destino
 * @param saida ficheiro de saída para gravar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool EncontrarCaminhos(Grafo* grafo, int origem, int destino, FILE* saida);

/**
 * @brief Mostra interseções entre antenas de frequências diferentes
 * @param grafo Apontador para o grafo de antenas
 * @param freqA Primeira frequência para comparação
 * @param freqB Segunda frequência para comparação
 * @param saida ficheiro de saída para gravar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool MostrarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida);

/**
 * @brief Calcula pontos de interferência entre antenas da mesma frequência
 * @param grafo Apontador para o grafo de antenas
 * @param saida ficheiro de saída para gravar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool CalcularInterferencias(Grafo* grafo, FILE* saida);

/**
 * @brief Exporta todos os resultados para um ficheiro
 * @param grafo Grafo contendo todas as antenas
 * @param nome_ficheiro Nome do ficheiro de saída
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool ExportarResultados(Grafo grafo, const char* nome_ficheiro);

/**
 * @brief Libera a memória alocada para o grafo
 * @param grafo Apontador para o grafo a ser liberado
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool LibertarGrafo(Grafo* grafo);

/**
 * @brief Limpa os flags de visitadas de todas as antenas
 * @param grafo Apontador para o grafo a ser modificado
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool LimparVisitados(Grafo* grafo);

#endif