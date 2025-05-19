/**
 * @file funcoes.h
 * @brief Ficheiro de cabeçalho com as definições e funções para gestão do grafo de antenas
 */

#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/**
 * @brief Estrutura que representa uma ligação entre antenas
 */
typedef struct Adjacencia {
    struct Antena* destino;     ///< Apontador para a antena de destino
    struct Adjacencia* proxima; ///< Apontador para a próxima adjacência na lista
} Adjacencia;

/**
 * @brief Estrutura que representa uma antena na rede
 */
typedef struct Antena {
    char frequencia;            ///< Frequência de operação da antena (carácter único)
    int coluna;                 ///< Posição horizontal na matriz (coordenada X)
    int linha;                  ///< Posição vertical na matriz (coordenada Y)
    Adjacencia* conexoes;       ///< Lista de ligações a outras antenas
    bool visitada;              ///< Indicador se a antena foi visitada em algoritmos de travessia
    struct Antena* proxima;     ///< Apontador para a próxima antena na lista do grafo
} Antena;

/**
 * @brief Estrutura para armazenar caminhos entre antenas
 */
typedef struct CaminhoNode {
    Antena* antena;             ///< Ponteiro para a antena no caminho
    struct CaminhoNode* proxima; ///< Ponteiro para o próximo nó do caminho
} CaminhoNode;

/**
 * @brief Estrutura para implementação de fila em BFSS
 */
typedef struct FilaNode {
    Antena* antena;             ///< Ponteiro para a antena na fila
    struct FilaNode* proxima;    ///< Ponteiro para o próximo nó da fila
} FilaNode;

/**
 * @brief Estrutura principal que representa o grafo de antenas
 */
typedef struct Grafo {
    Antena* antenas;            ///< Apontador para a primeira antena do grafo
    int total_antenas;          ///< Número total de antenas no grafo
} Grafo;

/**
 * @brief Adiciona uma ligação entre duas antenas
 * @param origem Apontador para a antena de origem
 * @param destino Apontador para a antena de destino
 * @return Verdadeiro se a ligação foi criada com sucesso, falso caso contrário
 */
bool AdicionarAdjacencia(Antena* origem, Antena* destino);

/**
 * @brief Adiciona uma nova antena ao grafo
 * @param grafo Apontador para o grafo onde será adicionada a antena
 * @param freq Frequência da nova antena
 * @param col Posição horizontal da antena
 * @param lin Posição vertical da antena
 * @return Verdadeiro se a antena foi adicionada com sucesso, falso caso contrário
 */
bool AdicionarAntena(Grafo* grafo, char freq, int col, int lin);

/**
 * @brief Carrega as antenas a partir de um ficheiro de texto
 * @param nome_ficheiro Nome do ficheiro contendo os dados das antenas
 * @return Estrutura Grafo populada com as antenas e suas ligações
 */
Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro);

/**
 * @brief Realiza uma travessia em profundidade (DFS) no grafo
 * @param grafo Apontador para o grafo a ser percorrido
 * @param inicio Apontador para a antena inicial da travessia
 * @param saida Ficheiro onde serão escritos os resultados
 * @return Verdadeiro se a travessia foi concluída com sucesso, falso caso contrário
 */
bool ProcuraEmProfundidade(Grafo* grafo, Antena* inicio, FILE* saida);

/**
 * @brief Realiza uma travessia em largura (BFS) no grafo
 * @param grafo Apontador para o grafo a ser percorrido
 * @param inicio Apontador para a antena inicial da travessia
 * @param saida Ficheiro onde serão escritos os resultados
 * @return Verdadeiro se a travessia foi concluída com sucesso, falso caso contrário
 */
bool ProcuraEmLargura(Grafo* grafo, Antena* inicio, FILE* saida);

/**
 * @brief Encontra todos os caminhos entre duas antenas
 * @param grafo Apontador para o grafo a ser analisado
 * @param origem Apontador para a antena de origem
 * @param destino Apontador para a antena de destino
 * @param saida Ficheiro onde serão escritos os resultados
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool EncontrarCaminhos(Grafo* grafo, Antena* origem, Antena* destino, FILE* saida);

/**
 * @brief Mostra intersecções entre antenas de frequências diferentes
 * @param grafo Apontador para o grafo a ser analisado
 * @param freqA Primeira frequência para comparação
 * @param freqB Segunda frequência para comparação
 * @param saida Ficheiro onde serão escritos os resultados
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool MostrarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida);

/**
 * @brief Calcula pontos de interferência entre antenas
 * @param grafo Apontador para o grafo a ser analisado
 * @param saida Ficheiro onde serão escritos os resultados
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool CalcularInterferencias(Grafo* grafo, FILE* saida);

/**
 * @brief Exporta todos os resultados para um ficheiro
 * @param grafo Grafo a ser analisado
 * @param nome_ficheiro Nome do ficheiro de saída
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool ExportarResultados(Grafo grafo, const char* nome_ficheiro);

/**
 * @brief Liberta toda a memória alocada para o grafo
 * @param grafo Apontador para o grafo a ser libertado
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool LibertarGrafo(Grafo* grafo);

/**
 * @brief Limpa os marcadores de visita de todas as antenas
 * @param grafo Apontador para o grafo a ser reiniciado
 * @return Verdadeiro se a operação foi concluída com sucesso, falso caso contrário
 */
bool LimparVisitados(Grafo* grafo);

#endif