/**
 * @file funcoes.c
 * @brief Implementação das funções de gestão
 * @author  Tiago Fontes
 */

 #include "funcoes.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /**
 * @brief Cria e inicializa uma nova estrutura de antena
 * @param coluna Posição horizontal no grid (coordenada x)
 * @param linha Posição vertical no grid (coordenada y)
 * @param freq Caractere representando a frequência da antena
 * @return Ponteiro para a antena criada ou NULL em caso de falha
 * @warning A memória alocada deve ser liberada com LibertarAntenas()
 */
 antena* CriarAntena(int coluna, int linha, char freq) {
     antena* nova = (antena*)malloc(sizeof(antena));
     if(!nova) return NULL;
     
     nova->coluna = coluna;
     nova->linha = linha;
     nova->frequencia = freq;
     nova->prox = NULL;
     return nova;
 }
 
 /**
 * @brief Carrega uma lista de antenas a partir de um ficheiro de texto
 * @param nome_ficheiro Caminho para o ficheiro contendo os dados das antenas
 * @return Lista encadeada de antenas ou NULL em caso de erro
 * @details O ficheiro deve seguir o formato:
 *          - Primeira linha: total_linhas total_colunas
 *          - Linhas subsequentes: representação do grid ('.' para vazio)
 * @warning A memória alocada deve ser liberada com LibertarAntenas()
 */
 antena* CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
     FILE* fp = fopen(nome_ficheiro, "r");
     if(fp == NULL) {
         fprintf(stderr, "Erro ao abrir %s\n", nome_ficheiro);
         return NULL;
     }
 
     int total_linhas, total_colunas;
     if(fscanf(fp, "%d %d", &total_linhas, &total_colunas) != 2) {
         fclose(fp);
         fprintf(stderr, "Formato inválido\n");
         return NULL;
     }
 
     antena* lista = NULL;
     antena* ultima = NULL;
     char linha[MAX_COLUNAS + 2];
     int y = 0;
 
     while(fgets(linha, sizeof(linha), fp) && y < total_linhas) {
         linha[strcspn(linha, "\n")] = '\0';
         for(int x = 0; x < total_colunas && x < (int)strlen(linha); x++) {
             if(linha[x] != '.' && linha[x] != ' ') {
                 antena* ant = CriarAntena(x, y, linha[x]);
                 if(!ant) continue;
                 
                 if(!lista) lista = ant;
                 else ultima->prox = ant;
                 ultima = ant;
             }
         }
         y++;
     }
     fclose(fp);
     return lista;
 }
 
 /**
 * @brief Calcula pontos de interferência entre antenas de mesma frequência
 * @param lista Lista encadeada de antenas a serem analisadas
 * @return Lista de pontos com interferência ou NULL se nenhuma for encontrada
 * @details Para cada par de antenas com mesma frequência, calcula os pontos
 *          de interferência projetando o vetor entre elas em ambas as direções
 * @warning A memória alocada deve ser liberada com LibertarEfeitosNefastos()
 */
 nefasto* CalcularEfeitosNefastos(antena* lista) {
     nefasto* nefastos = NULL;
     antena* atual = lista;
 
     while(atual) {
         antena* comparar = atual->prox;
         while(comparar) {
             if(atual->frequencia == comparar->frequencia) {
                 int diff_linha = comparar->linha - atual->linha;
                 int diff_coluna = comparar->coluna - atual->coluna;
 
                 // Direção comparar -> futuro
                 int nefasto_linha = comparar->linha + diff_linha;
                 int nefasto_coluna = comparar->coluna + diff_coluna;
 
                 if(nefasto_linha >= 0 && nefasto_coluna >= 0 &&
                    nefasto_linha < MAX_LINHAS && nefasto_coluna < MAX_COLUNAS) {
                     nefasto* novo = (nefasto*)malloc(sizeof(nefasto));
                     if(novo) {
                         novo->coluna = nefasto_coluna;
                         novo->linha = nefasto_linha;
                         novo->prox = nefastos;
                         nefastos = novo;
                     }
                 }
 
                 // Direção atual -> passado
                 nefasto_linha = atual->linha - diff_linha;
                 nefasto_coluna = atual->coluna - diff_coluna;
 
                 if(nefasto_linha >= 0 && nefasto_coluna >= 0 &&
                    nefasto_linha < MAX_LINHAS && nefasto_coluna < MAX_COLUNAS) {
                     nefasto* novo = (nefasto*)malloc(sizeof(nefasto));
                     if(novo) {
                         novo->coluna = nefasto_coluna;
                         novo->linha = nefasto_linha;
                         novo->prox = nefastos;
                         nefastos = novo;
                     }
                 }
             }
             comparar = comparar->prox;
         }
         atual = atual->prox;
     }
     return nefastos;
 }
 
 /**
 * @brief Libera a memória alocada para uma lista de antenas
 * @param lista Ponteiro para o início da lista de antenas
 */
 void LibertarAntenas(antena* lista) {
     while(lista) {
         antena* temp = lista;
         lista = lista->prox;
         free(temp);
     }
 }
 
 /**
 * @brief Libera a memória alocada para uma lista de efeitos nefastos
 * @param lista Ponteiro para o início da lista de efeitos nefastos
 */
 void LibertarEfeitosNefastos(nefasto* lista) {
     while(lista) {
         nefasto* temp = lista;
         lista = lista->prox;
         free(temp);
     }
 }