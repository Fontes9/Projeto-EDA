/**
 * @file funcoes.c
 * @brief Implementação das funções para gestão do grafo de antenas
 */

#include "funcoes.h"

/**
 * @brief Adiciona uma ligação entre duas antenas
 * @param origem Apontador para a antena de origem
 * @param destino Apontador para a antena de destino
 * @return true se a ligação foi criada com sucesso, false caso contrário
 * @note A ligação é adicionada no início da lista de adjacências
 */
bool AdicionarAdjacencia(Antena* origem, Antena* destino) {
    if (!origem || !destino) return false;
    
    Adjacencia* nova = malloc(sizeof(Adjacencia));
    if (!nova) return false;
    
    nova->destino = destino;
    nova->proxima = origem->conexoes;
    origem->conexoes = nova;
    return true;
}

/**
 * @brief Adiciona uma nova antena ao grafo
 * @param grafo Apontador para o grafo
 * @param freq Frequência da antena (carácter único)
 * @param col Posição horizontal (coluna)
 * @param lin Posição vertical (linha)
 * @return true se a antena foi adicionada com sucesso, false caso contrário
 * @note A antena é adicionada no início da lista ligada
 */
bool AdicionarAntena(Grafo* grafo, char freq, int col, int lin) {
    if (!grafo) return false;
    
    Antena* nova = malloc(sizeof(Antena));
    if (!nova) return false;
    
    nova->frequencia = freq;
    nova->coluna = col;
    nova->linha = lin;
    nova->conexoes = NULL;
    nova->visitada = false;
    nova->proxima = grafo->antenas;
    grafo->antenas = nova;
    grafo->total_antenas++;
    return true;
}

/**
 * @brief Carrega a rede de antenas a partir de um ficheiro
 * @param nome_ficheiro Nome do ficheiro de entrada
 * @return Estrutura Grafo populada
 * @note Formato do ficheiro:
 *       - Primeira linha: linhas colunas
 *       - Linhas seguintes: matriz de caracteres
 *       - Caracteres válidos: qualquer exceto '.' e ' '
 */
Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
    Grafo grafo = { NULL, 0 };
    
    if (!nome_ficheiro) return grafo;
    
    FILE* ficheiro = fopen(nome_ficheiro, "r");
    if (!ficheiro) {
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

    char linha[1024]; // Buffer fixo alternativo
    for (int y = 0; y < linhas && fgets(linha, sizeof(linha), ficheiro) != NULL; y++) {
        linha[strcspn(linha, "\n")] = '\0'; // Remove newline
        for (int x = 0; x < colunas && x < (int)strlen(linha); x++) {
            if (linha[x] != '.' && linha[x] != ' ') {
                AdicionarAntena(&grafo, linha[x], x, y);
            }
        }
    }
    
    fclose(ficheiro);

    // Restante da função permanece igual
    for (Antena* a1 = grafo.antenas; a1 != NULL; a1 = a1->proxima) {
        for (Antena* a2 = grafo.antenas; a2 != NULL; a2 = a2->proxima) {
            if (a1 != a2 && a1->frequencia == a2->frequencia) {
                AdicionarAdjacencia(a1, a2);
            }
        }
    }
    
    return grafo;
}

/**
 * @brief Função auxiliar para busca em profundidade (DFS)
 * @param grafo Apontador para o grafo
 * @param atual Apontador para a antena atual
 * @param saida Ficheiro de saída para resultados
 * @return true se a operação foi bem sucedida
 */
bool ProcuraEmProfundidade(Grafo* grafo, Antena* atual, FILE* saida) {
    if (!grafo || !atual || !saida) return false;
    if (atual->visitada) return true;
    
    atual->visitada = true;
    fprintf(saida, "Antena %c @ (%d,%d)\n", atual->frequencia, atual->coluna, atual->linha);

    for (Adjacencia* adj = atual->conexoes; adj != NULL; adj = adj->proxima) {
        ProcuraEmProfundidade(grafo, adj->destino, saida);
    }
    return true;
}

/**
 * @brief Função auxiliar para busca em largura (BFS)
 * @param grafo Apontador para o grafo
 * @param inicio Apontador para a antena inicial
 * @param saida Ficheiro de saída para resultados
 * @return true se a operação foi bem sucedida
 * @note Utiliza uma fila implementada com lista ligada
 */
bool ProcuraEmLargura(Grafo* grafo, Antena* inicio, FILE* saida) {
    if (!grafo || !inicio || !saida) return false;
    
    FilaNode* frente = NULL;
    FilaNode* tras = NULL;
    
    FilaNode* novo = malloc(sizeof(FilaNode));
    if (!novo) return false;
    novo->antena = inicio;
    novo->proxima = NULL;
    
    frente = tras = novo;
    inicio->visitada = true;
    
    while (frente) {
        Antena* atual = frente->antena;
        FilaNode* temp = frente;
        frente = frente->proxima;
        free(temp);
        
        fprintf(saida, "Antena %c @ (%d,%d)\n", atual->frequencia, atual->coluna, atual->linha);
        
        for (Adjacencia* adj = atual->conexoes; adj != NULL; adj = adj->proxima) {
            if (!adj->destino->visitada) {
                adj->destino->visitada = true;
                FilaNode* novo_node = malloc(sizeof(FilaNode));
                if (!novo_node) continue;
                novo_node->antena = adj->destino;
                novo_node->proxima = NULL;
                
                if (!frente) frente = novo_node;
                else tras->proxima = novo_node;
                tras = novo_node;
            }
        }
    }
    return true;
}

/**
 * @brief Função auxiliar para imprimir um caminho
 * @param caminho Apontador para o início do caminho
 * @param saida Ficheiro de saída
 * @note Função recursiva que imprime do fim para o início
 */
static void ImprimirCaminho(CaminhoNode* caminho, FILE* saida) {
    if (!caminho) return;
    ImprimirCaminho(caminho->proxima, saida);
    fprintf(saida, "%c(%d,%d)", caminho->antena->frequencia, caminho->antena->coluna, caminho->antena->linha);
    if (caminho->proxima) fprintf(saida, " -> ");
}

/**
 * @brief Função auxiliar recursiva para encontrar caminhos
 * @param grafo Apontador para o grafo
 * @param atual Antena atual no caminho
 * @param destino Antena de destino
 * @param caminho Caminho acumulado
 * @param saida Ficheiro de saída
 * @return true se a operação foi bem sucedida
 */
bool EncontrarCaminhosRec(Grafo* grafo, Antena* atual, Antena* destino, CaminhoNode* caminho, FILE* saida) {
    CaminhoNode* novo = malloc(sizeof(CaminhoNode));
    if (!novo) return false;
    novo->antena = atual;
    novo->proxima = caminho;
    caminho = novo;
    
    atual->visitada = true;
    
    if (atual == destino) {
        ImprimirCaminho(caminho, saida);
        fprintf(saida, "\n");
    } else {
        for (Adjacencia* adj = atual->conexoes; adj != NULL; adj = adj->proxima) {
            if (!adj->destino->visitada) {
                EncontrarCaminhosRec(grafo, adj->destino, destino, caminho, saida);
            }
        }
    }
    
    atual->visitada = false;
    free(caminho);
    return true;
}

/**
 * @brief Encontra todos os caminhos entre duas antenas
 * @param grafo Apontador para o grafo
 * @param origem Antena de origem
 * @param destino Antena de destino
 * @param saida Ficheiro de saída
 * @return true se a operação foi bem sucedida
 */
bool EncontrarCaminhos(Grafo* grafo, Antena* origem, Antena* destino, FILE* saida) {
    if (!grafo || !origem || !destino || !saida) return false;
    LimparVisitados(grafo);
    return EncontrarCaminhosRec(grafo, origem, destino, NULL, saida);
}

/**
 * @brief Mostra intersecções entre antenas de frequências diferentes
 * @param grafo Apontador para o grafo
 * @param freqA Primeira frequência
 * @param freqB Segunda frequência
 * @param saida Ficheiro de saída
 * @return true se a operação foi bem sucedida
 */
bool MostrarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida) {
    if (!grafo || !saida) return false;
    
    for (Antena* a = grafo->antenas; a != NULL; a = a->proxima) {
        if (a->frequencia == freqA) {
            for (Antena* b = grafo->antenas; b != NULL; b = b->proxima) {
                if (b->frequencia == freqB && 
                    (a->linha == b->linha || a->coluna == b->coluna)) {
                    fprintf(saida, "%c(%d,%d) - %c(%d,%d)\n",
                           freqA, a->coluna, a->linha,
                           freqB, b->coluna, b->linha);
                }
            }
        }
    }
    return true;
}

/**
 * @brief Calcula pontos de interferência entre antenas
 * @param grafo Apontador para o grafo
 * @param saida Ficheiro de saída
 * @return true se a operação foi bem sucedida
 */
bool CalcularInterferencias(Grafo* grafo, FILE* saida) {
    if (!grafo || !saida) return false;
    
    fprintf(saida, "=== PONTOS DE INTERFERENCIA ===\n");
    
    for (Antena* a1 = grafo->antenas; a1 != NULL; a1 = a1->proxima) {
        for (Antena* a2 = grafo->antenas; a2 != NULL; a2 = a2->proxima) {
            if (a1->frequencia == a2->frequencia) {
                int dx = a2->coluna - a1->coluna;
                int dy = a2->linha - a1->linha;

                if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) {
                    for (int k = 1; k < 3; k++) {
                        int x = a1->coluna + (k * dx) / 3;
                        int y = a1->linha + (k * dy) / 3;
                        fprintf(saida, "(%d,%d)\n", x, y);
                    }
                }
            }
        }
    }
    return true;
}

/**
 * @brief Exporta todos os resultados para um ficheiro
 * @param grafo Grafo a ser analisado
 * @param nome_ficheiro Nome do ficheiro de saída
 * @return true se a operação foi bem sucedida
 */
bool ExportarResultados(Grafo grafo, const char* nome_ficheiro) {
    if (!nome_ficheiro) return false;
    
    FILE* saida = fopen(nome_ficheiro, "w");
    if (!saida) {
        perror("Erro ao criar ficheiro");
        return false;
    }

    fprintf(saida, "=== ANTENAS (%d) ===\n", grafo.total_antenas);
    for (Antena* a = grafo.antenas; a != NULL; a = a->proxima) {
        fprintf(saida, "%c @ (%d,%d)\n", a->frequencia, a->coluna, a->linha);
    }

    if (grafo.total_antenas > 0) {
        Antena* primeira = grafo.antenas;
        
        fprintf(saida, "\n=== BUSCA EM PROFUNDIDADE ===\n");
        ProcuraEmProfundidade(&grafo, primeira, saida);
        LimparVisitados(&grafo);

        fprintf(saida, "\n=== BUSCA EM LARGURA ===\n");
        ProcuraEmLargura(&grafo, primeira, saida);
        LimparVisitados(&grafo);

        if (grafo.total_antenas > 2) {
            Antena* terceira = primeira->proxima->proxima;
            fprintf(saida, "\n=== CAMINHOS ENTRE ANTENAS ===\n");
            EncontrarCaminhos(&grafo, primeira, terceira, saida);
        }

        fprintf(saida, "\n=== INTERSECOES ===\n");
        for (Antena* a1 = grafo.antenas; a1 != NULL; a1 = a1->proxima) {
            for (Antena* a2 = grafo.antenas; a2 != NULL; a2 = a2->proxima) {
                if (a1->frequencia != a2->frequencia) {
                    MostrarIntersecoes(&grafo, a1->frequencia, a2->frequencia, saida);
                }
            }
        }

        fprintf(saida, "\n=== INTERFERENCIAS ===\n");
        CalcularInterferencias(&grafo, saida);
    }
    
    fclose(saida);
    return true;
}

/**
 * @brief Liberta toda a memória alocada pelo grafo
 * @param grafo Apontador para o grafo
 * @return true se a operação foi bem sucedida
 */
bool LibertarGrafo(Grafo* grafo) {
    if (!grafo) return false;
    
    Antena* atual = grafo->antenas;
    while (atual) {
        Adjacencia* adj = atual->conexoes;
        while (adj) {
            Adjacencia* temp = adj;
            adj = adj->proxima;
            free(temp);
        }
        
        Antena* temp = atual;
        atual = atual->proxima;
        free(temp);
    }
    
    grafo->antenas = NULL;
    grafo->total_antenas = 0;
    return true;
}

/**
 * @brief Reinicia os marcadores de visita das antenas
 * @param grafo Apontador para o grafo
 * @return true se a operação foi bem sucedida
 */
bool LimparVisitados(Grafo* grafo) {
    if (!grafo) return false;
    for (Antena* a = grafo->antenas; a != NULL; a = a->proxima) {
        a->visitada = false;
    }
    return true;
}