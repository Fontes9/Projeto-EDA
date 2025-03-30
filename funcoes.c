#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

antena* CriarAntena(int coluna, int linha, char freq) {
    antena* nova = (antena*)malloc(sizeof(antena));
    if (nova) {
        nova->coluna = coluna;
        nova->linha = linha;
        nova->frequencia = freq;
        nova->prox = NULL;
    }
    return nova;
}

nefasto* CriarEfeitoNefasto(int coluna, int linha) {
    nefasto* novo = (nefasto*)malloc(sizeof(nefasto));
    if (novo) {
        novo->coluna = coluna;
        novo->linha = linha;
        novo->prox = NULL;
    }
    return novo;
}

antena* CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
    FILE* fp = fopen(nome_ficheiro, "r");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro\n");
        return NULL;
    }

    // Lê as dimensões da grelha
    int total_linhas, total_colunas;
    if (fscanf(fp, "%d %d", &total_linhas, &total_colunas) != 2) {
        fclose(fp);
        return NULL;
    }

    // Consome o resto da primeira linha
    while (fgetc(fp) != '\n' && !feof(fp));

    antena* lista = NULL;
    antena* ultima = NULL;
    char linha[256];
    int y = 0;  // Linha atual (base 0)

    while (fgets(linha, sizeof(linha), fp) && y < total_linhas) {
        // Remove a quebra de linha no final
        linha[strcspn(linha, "\n")] = '\0';
        
        for (int x = 0; x < total_colunas && x < (int)strlen(linha); x++) {
            if (linha[x] != '.' && linha[x] != ' ') {
                antena* ant = CriarAntena(x, y, linha[x]);
                
                if (lista == NULL) {
                    lista = ant;
                } else {
                    ultima->prox = ant;
                }
                ultima = ant;
            }
        }
        y++;  // Avança para a próxima linha
    }

    fclose(fp);
    return lista;
}

nefasto* CalcularEfeitosNefastos(antena* lista) {
    nefasto* lista_nefastos = NULL;
    antena* atual = lista;

    while (atual != NULL) {
        antena* comparar = atual->prox;

        while (comparar != NULL) {
            if (atual->frequencia == comparar->frequencia) {
                
                int diff_linha = comparar->linha - atual->linha;
                int diff_coluna = comparar->coluna - atual->coluna;

                // Calcula efeito após o ponto de comparação
                int linha_nefasta = comparar->linha + diff_linha;
                int coluna_nefasta = comparar->coluna + diff_coluna;

                // Verifica se está dentro dos limites
                if (linha_nefasta >= 0 && coluna_nefasta >= 0 && 
                    linha_nefasta < MAX_LINHAS && coluna_nefasta < MAX_COLUNAS) {
                    nefasto* novo = CriarEfeitoNefasto(coluna_nefasta, linha_nefasta);
                    novo->prox = lista_nefastos;
                    lista_nefastos = novo;
                }

                // Calcula efeito antes do ponto atual
                linha_nefasta = atual->linha - diff_linha;
                coluna_nefasta = atual->coluna - diff_coluna;

                if (linha_nefasta >= 0 && coluna_nefasta >= 0 && 
                    linha_nefasta < MAX_LINHAS && coluna_nefasta < MAX_COLUNAS) {
                    nefasto* novo2 = CriarEfeitoNefasto(coluna_nefasta, linha_nefasta);
                    novo2->prox = lista_nefastos;
                    lista_nefastos = novo2;
                }
            }
            comparar = comparar->prox;
        }
        atual = atual->prox;
    }

    return lista_nefastos;
}

// Funções auxiliares
void LibertarAntenas(antena* lista) {
    while (lista) {
        antena* temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

void LibertarEfeitosNefastos(nefasto* lista) {
    while (lista) {
        nefasto* temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

void ListarAntenas(antena* lista) {
    printf("Lista de Antenas (coluna,linha):\n");
    while (lista) {
        printf("Freq: %c @ (%d,%d)\n", 
              lista->frequencia, lista->coluna, lista->linha);
        lista = lista->prox;
    }
}

void ListarEfeitosNefastos(nefasto* lista) {
    printf("Efeitos Nefastos:\n");
    while (lista) {
        printf("Posição: (%d,%d)\n", lista->coluna, lista->linha);
        lista = lista->prox;
    }
}