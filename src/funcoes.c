/**
 * @file funcoes.c
 * @brief Implementação das funções para gestão de antenas
 * @author [Seu Nome]
 * @date 2023-06-15
 */

 #include "funcoes.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 // ------------------------------------
 // Funções principais
 // ------------------------------------
 
 antena* CriarAntena(int coluna, int linha, char freq) {
     antena* nova = (antena*)malloc(sizeof(antena));
     if(!nova) return NULL;
     
     nova->coluna = coluna;
     nova->linha = linha;
     nova->frequencia = freq;
     nova->prox = NULL;
     return nova;
 }
 
 antena* CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
     FILE* fp = fopen(nome_ficheiro, "r");
     if(!fp) {
         fprintf(stderr, "Erro: não foi possível abrir %s\n", nome_ficheiro);
         return NULL;
     }
 
     int total_linhas, total_colunas;
     if(fscanf(fp, "%d %d", &total_linhas, &total_colunas) != 2) {
         fclose(fp);
         fprintf(stderr, "Erro: formato inválido nas dimensões\n");
         return NULL;
     }
 
     // Consome resto da linha
     while(fgetc(fp) != '\n' && !feof(fp));
 
     antena* lista = NULL;
     antena* ultima = NULL;
     char linha[MAX_COLUNAS + 2]; // +2 para \n e \0
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
 
 nefasto* CalcularEfeitosNefastos(antena* lista) {
     nefasto* nefastos = NULL;
     antena* atual = lista;
 
     while(atual) {
         antena* comparar = atual->prox;
         while(comparar) {
             if(atual->frequencia == comparar->frequencia) {
                 int diff_linha = comparar->linha - atual->linha;
                 int diff_coluna = comparar->coluna - atual->coluna;
 
                 // Verifica efeito na direção comparar -> futuro
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
 
                 // Verifica efeito na direção atual -> passado
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
 
 // ------------------------------------
 // Funções auxiliares
 // ------------------------------------
 
 void LibertarAntenas(antena* lista) {
     while(lista) {
         antena* temp = lista;
         lista = lista->prox;
         free(temp);
     }
 }
 
 void LibertarEfeitosNefastos(nefasto* lista) {
     while(lista) {
         nefasto* temp = lista;
         lista = lista->prox;
         free(temp);
     }
 }