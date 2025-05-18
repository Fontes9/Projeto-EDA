/**
 * @file funcoes.c
 * @brief Implementação das funções para gestão do grafo de antenas
 */

#include "funcoes.h"

/**
 * @brief Adiciona uma ligação entre antenas
 * @param antena Apontador para a antena de origem
 * @param destino Índice da antena de destino
 * @return true se a ligação foi criada com sucesso, false caso contrário
 */

bool AdicionarAdjacencia(Antena* antena, int destino) {
    Adjacencia* nova = (Adjacencia*)malloc(sizeof(Adjacencia));
    if (nova == NULL) return false;
    
    nova->destino = destino;
    nova->proxima = antena->conexoes;
    antena->conexoes = nova;
    return true;
}

Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
    FILE* ficheiro = fopen(nome_ficheiro, "r");
    Grafo grafo = { .total_antenas = 0 };

    if (ficheiro == NULL) {
        perror("Erro ao abrir ficheiro");
        return grafo;
    }

    int linhas, colunas;
    if (fscanf(ficheiro, "%d %d", &linhas, &colunas) != 2) {
        fclose(ficheiro);
        fprintf(stderr, "Formato de ficheiro inválido\n");
        return grafo;
    }

    while (fgetc(ficheiro) != '\n');

    char linha[MAX_COLUNAS + 2];
    for (int y = 0; y < linhas && fgets(linha, sizeof(linha), ficheiro) != NULL; y++) {
        linha[strcspn(linha, "\n")] = '\0';
        for (int x = 0; x < colunas && x < (int)strlen(linha); x++) {
            if (linha[x] != '.' && linha[x] != ' ') {
                grafo.antenas[grafo.total_antenas].frequencia = linha[x];
                grafo.antenas[grafo.total_antenas].coluna = x;
                grafo.antenas[grafo.total_antenas].linha = y;
                grafo.antenas[grafo.total_antenas].conexoes = NULL;
                grafo.antenas[grafo.total_antenas].visitada = false;
                grafo.total_antenas++;
            }
        }
    }
    fclose(ficheiro);

    for (int i = 0; i < grafo.total_antenas; i++) {
        for (int j = i + 1; j < grafo.total_antenas; j++) {
            if (grafo.antenas[i].frequencia == grafo.antenas[j].frequencia) {
                AdicionarAdjacencia(&grafo.antenas[i], j);
                AdicionarAdjacencia(&grafo.antenas[j], i);
            }
        }
    }

    return grafo;
}

bool LimparVisitados(Grafo* grafo) {
    if (grafo == NULL) return false;
    for (int i = 0; i < grafo->total_antenas; i++) {
        grafo->antenas[i].visitada = false;
    }
    return true;
}

bool ProcuraEmProfundidade(Grafo* grafo, int atual, FILE* saida) {
    if (grafo == NULL || atual < 0 || atual >= grafo->total_antenas || saida == NULL) {
        return false;
    }
    
    grafo->antenas[atual].visitada = true;
    Antena a = grafo->antenas[atual];
    fprintf(saida, "Antena %c @ (%d,%d)\n", a.frequencia, a.coluna, a.linha);

    for (Adjacencia* adj = a.conexoes; adj != NULL; adj = adj->proxima) {
        if (!grafo->antenas[adj->destino].visitada) {
            ProcuraEmProfundidade(grafo, adj->destino, saida);
        }
    }
    return true;
}

bool ProcuraEmLargura(Grafo* grafo, int inicio, FILE* saida) {
    if (grafo == NULL || inicio < 0 || inicio >= grafo->total_antenas || saida == NULL) {
        return false;
    }
    
    int fila[MAX_ANTENAS];
    int frente = 0, tras = 0;
    
    grafo->antenas[inicio].visitada = true;
    fila[tras++] = inicio;

    while (frente < tras) {
        int atual = fila[frente++];
        Antena a = grafo->antenas[atual];
        fprintf(saida, "Antena %c @ (%d,%d)\n", a.frequencia, a.coluna, a.linha);

        for (Adjacencia* adj = a.conexoes; adj != NULL; adj = adj->proxima) {
            if (!grafo->antenas[adj->destino].visitada) {
                grafo->antenas[adj->destino].visitada = true;
                fila[tras++] = adj->destino;
            }
        }
    }
    return true;
}

/**
 * @brief Função auxiliar recursiva para encontrar caminhos
 * @param grafo Apontador para o grafo de antenas
 * @param atual Índice da antena atual no caminho
 * @param destino Índice da antena de destino
 * @param visitados Array de antenas visitadas
 * @param caminho Array com os índices do caminho atual
 * @param pos Posição atual no caminho
 * @param saida Ficheiro de saída para registar os resultados
 * @return true se a operação foi bem sucedida, false caso contrário
 */

bool EncontrarCaminhosRec(Grafo* grafo, int atual, int destino, bool visitados[], int caminho[], int pos, FILE* saida) {
    visitados[atual] = true;
    caminho[pos] = atual;
    pos++;

    if (atual == destino) {
        for (int i = 0; i < pos; i++) {
            Antena a = grafo->antenas[caminho[i]];
            fprintf(saida, "%c(%d,%d)", a.frequencia, a.coluna, a.linha);
            if (i < pos - 1) fprintf(saida, " -> ");
        }
        fprintf(saida, "\n");
    } else {
        for (Adjacencia* adj = grafo->antenas[atual].conexoes; adj != NULL; adj = adj->proxima) {
            if (!visitados[adj->destino]) {
                EncontrarCaminhosRec(grafo, adj->destino, destino, visitados, caminho, pos, saida);
            }
        }
    }

    visitados[atual] = false;
    return true;
}

bool EncontrarCaminhos(Grafo* grafo, int origem, int destino, FILE* saida) {
    if (grafo == NULL || origem < 0 || origem >= grafo->total_antenas || 
        destino < 0 || destino >= grafo->total_antenas || saida == NULL) {
        return false;
    }

    bool visitados[MAX_ANTENAS] = {false};
    int caminho[MAX_ANTENAS];
    EncontrarCaminhosRec(grafo, origem, destino, visitados, caminho, 0, saida);
    return true;
}

bool MostrarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida) {
    if (grafo == NULL || saida == NULL) return false;
    
    for (int i = 0; i < grafo->total_antenas; i++) {
        if (grafo->antenas[i].frequencia == freqA) {
            for (int j = 0; j < grafo->total_antenas; j++) {
                if (grafo->antenas[j].frequencia == freqB) {
                    if (grafo->antenas[i].linha == grafo->antenas[j].linha || 
                        grafo->antenas[i].coluna == grafo->antenas[j].coluna) {
                        fprintf(saida, "%c(%d,%d) - %c(%d,%d)\n",
                               freqA, grafo->antenas[i].coluna, grafo->antenas[i].linha,
                               freqB, grafo->antenas[j].coluna, grafo->antenas[j].linha);
                    }
                }
            }
        }
    }
    return true;
}

bool CalcularInterferencias(Grafo* grafo, FILE* saida) {
    if (grafo == NULL || saida == NULL) return false;
    
    fprintf(saida, "=== PONTOS DE INTERFERENCIA ===\n");
    
    for (int i = 0; i < grafo->total_antenas; i++) {
        for (int j = i + 1; j < grafo->total_antenas; j++) {
            if (grafo->antenas[i].frequencia == grafo->antenas[j].frequencia) {
                int dx = grafo->antenas[j].coluna - grafo->antenas[i].coluna;
                int dy = grafo->antenas[j].linha - grafo->antenas[i].linha;

                if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) {
                    for (int k = 1; k < 3; k++) {
                        int x = grafo->antenas[i].coluna + (k * dx) / 3;
                        int y = grafo->antenas[i].linha + (k * dy) / 3;
                        
                        if (x >= 0 && x < MAX_COLUNAS && y >= 0 && y < MAX_LINHAS) {
                            fprintf(saida, "(%d,%d)\n", x, y);
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool ExportarResultados(Grafo grafo, const char* nome_ficheiro) {
    FILE* saida = fopen(nome_ficheiro, "w");
    if (saida == NULL) {
        perror("Erro ao criar ficheiro");
        return false;
    }

    fprintf(saida, "=== ANTENAS (%d) ===\n", grafo.total_antenas);
    for (int i = 0; i < grafo.total_antenas; i++) {
        fprintf(saida, "%d: %c @ (%d,%d)\n", 
               i, grafo.antenas[i].frequencia,
               grafo.antenas[i].coluna,
               grafo.antenas[i].linha);
    }

    if (grafo.total_antenas > 0) {
        fprintf(saida, "\n=== BUSCA EM PROFUNDIDADE (a partir da antena 0) ===\n");
        ProcuraEmProfundidade(&grafo, 0, saida);
        LimparVisitados(&grafo);

        fprintf(saida, "\n=== BUSCA EM LARGURA (a partir da antena 0) ===\n");
        ProcuraEmLargura(&grafo, 0, saida);
        LimparVisitados(&grafo);

        if (grafo.total_antenas > 2) {
            fprintf(saida, "\n=== CAMINHOS ENTRE ANTENAS 0 e 2 ===\n");
            EncontrarCaminhos(&grafo, 0, 2, saida);
        }

        fprintf(saida, "\n=== INTERSECOES ENTRE FREQUENCIAS ===\n");
        for (int i = 0; i < grafo.total_antenas; i++) {
            for (int j = i + 1; j < grafo.total_antenas; j++) {
                if (grafo.antenas[i].frequencia != grafo.antenas[j].frequencia) {
                    MostrarIntersecoes(&grafo, grafo.antenas[i].frequencia, 
                                     grafo.antenas[j].frequencia, saida);
                }
            }
        }

        fprintf(saida, "\n=== INTERFERENCIAS ===\n");
        CalcularInterferencias(&grafo, saida);
    }
    
    fclose(saida);
    return true;
}

bool LibertarGrafo(Grafo* grafo) {
    if (grafo == NULL) return false;
    
    for (int i = 0; i < grafo->total_antenas; i++) {
        Adjacencia* atual = grafo->antenas[i].conexoes;
        while (atual != NULL) {
            Adjacencia* temp = atual;
            atual = atual->proxima;
            free(temp);
        }
    }
    return true;
}