#include "funcoes.h"

// Adiciona uma ligação entre duas antenas
void AdicionarLigacao(Antena* antena, int id_destino) {
    Ligacao* nova = (Ligacao*)malloc(sizeof(Ligacao));
    nova->id_antena_destino = id_destino;
    nova->proxima = antena->conexoes;
    antena->conexoes = nova;
}

// Carrega o grafo a partir de um ficheiro
Grafo CarregarAntenasDoFicheiro(const char* nome_ficheiro) {
    FILE* ficheiro = fopen(nome_ficheiro, "r");
    Grafo grafo = { .total_antenas = 0 };

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

    char linha[MAX_COLUNAS + 2];
    for (int y = 0; fgets(linha, sizeof(linha), ficheiro) && y < linhas; y++) {
        linha[strcspn(linha, "\n")] = '\0'; // Remove a quebra de linha
        for (int x = 0; x < colunas && x < (int)strlen(linha); x++) {
            if (linha[x] != '.' && linha[x] != ' ') {
                grafo.antenas[grafo.total_antenas++] = (Antena){
                    .frequencia = linha[x],
                    .coluna = x,
                    .linha = y,
                    .conexoes = NULL,
                    .visitada = false
                };
            }
        }
    }
    fclose(ficheiro);

    // Conectar antenas da mesma frequência
    for (int i = 0; i < grafo.total_antenas; i++) {
        for (int j = i + 1; j < grafo.total_antenas; j++) {
            if (grafo.antenas[i].frequencia == grafo.antenas[j].frequencia) {
                AdicionarLigacao(&grafo.antenas[i], j);
                AdicionarLigacao(&grafo.antenas[j], i);
            }
        }
    }

    return grafo;
}

// Busca em Profundidade (DFS)
void DFS(Grafo* grafo, int id, FILE* saida) {
    grafo->antenas[id].visitada = true;
    Antena a = grafo->antenas[id];
    fprintf(saida, "Antena %c @ (%d,%d)\n", a.frequencia, a.coluna, a.linha);

    for (Ligacao* lig = a.conexoes; lig; lig = lig->proxima) {
        if (!grafo->antenas[lig->id_antena_destino].visitada) {
            DFS(grafo, lig->id_antena_destino, saida);
        }
    }
}

// Busca em Largura (BFS)
void BFS(Grafo* grafo, int inicio, FILE* saida) {
    int fila[MAX_ANTENAS];
    int frente = 0, tras = 0;
    
    grafo->antenas[inicio].visitada = true;
    fila[tras++] = inicio;

    while (frente < tras) {
        int atual = fila[frente++];
        Antena a = grafo->antenas[atual];
        fprintf(saida, "Antena %c @ (%d,%d)\n", a.frequencia, a.coluna, a.linha);

        for (Ligacao* lig = a.conexoes; lig; lig = lig->proxima) {
            if (!grafo->antenas[lig->id_antena_destino].visitada) {
                grafo->antenas[lig->id_antena_destino].visitada = true;
                fila[tras++] = lig->id_antena_destino;
            }
        }
    }
}

// Limpa os estados de visita (para reutilização)
void LimparVisitados(Grafo* grafo) {
    for (int i = 0; i < grafo->total_antenas; i++) {
        grafo->antenas[i].visitada = false;
    }
}

// Encontra todos os caminhos entre duas antenas
void EncontrarCaminhosRec(Grafo* grafo, int atual, int destino, bool visitados[], int caminho[], int pos, FILE* saida) {
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
        for (Ligacao* lig = grafo->antenas[atual].conexoes; lig; lig = lig->proxima) {
            if (!visitados[lig->id_antena_destino]) {
                EncontrarCaminhosRec(grafo, lig->id_antena_destino, destino, visitados, caminho, pos, saida);
            }
        }
    }

    visitados[atual] = false;
}

void EncontrarTodosCaminhos(Grafo* grafo, int origem, int destino, FILE* saida) {
    bool visitados[MAX_ANTENAS] = {false};
    int caminho[MAX_ANTENAS];
    EncontrarCaminhosRec(grafo, origem, destino, visitados, caminho, 0, saida);
}

// Lista interseções entre antenas de frequências diferentes
void ListarIntersecoes(Grafo* grafo, char freqA, char freqB, FILE* saida) {
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
}

// Calcula os efeitos nefastos (pontos de interferência)
void CalcularEfeitosNefastos(Grafo* grafo, FILE* saida) {
    for (int i = 0; i < grafo->total_antenas; i++) {
        for (Ligacao* lig = grafo->antenas[i].conexoes; lig; lig = lig->proxima) {
            int j = lig->id_antena_destino;
            if (i < j) { // Evitar duplicatas
                int dx = grafo->antenas[j].coluna - grafo->antenas[i].coluna;
                int dy = grafo->antenas[j].linha - grafo->antenas[i].linha;
                
                // Direção j
                int x = grafo->antenas[j].coluna + dx;
                int y = grafo->antenas[j].linha + dy;
                if (x >= 0 && y >= 0 && x < MAX_COLUNAS && y < MAX_LINHAS) {
                    fprintf(saida, "(%d,%d) - Frequencia: %c\n", x, y, grafo->antenas[i].frequencia);
                }

                // Direção i
                x = grafo->antenas[i].coluna - dx;
                y = grafo->antenas[i].linha - dy;
                if (x >= 0 && y >= 0 && x < MAX_COLUNAS && y < MAX_LINHAS) {
                    fprintf(saida, "(%d,%d) - Frequencia: %c\n", x, y, grafo->antenas[i].frequencia);
                }
            }
        }
    }
}

// Exporta todos os resultados para um ficheiro
void ExportarResultados(Grafo grafo, const char* nome_ficheiro) {
    FILE* saida = fopen(nome_ficheiro, "w");
    if (!saida) {
        perror("Erro ao criar ficheiro");
        return;
    }

    fprintf(saida, "=== ANTENAS ===\n");
    for (int i = 0; i < grafo.total_antenas; i++) {
        fprintf(saida, "ID %d: %c @ (%d,%d)\n", i, 
               grafo.antenas[i].frequencia,
               grafo.antenas[i].coluna,
               grafo.antenas[i].linha);
    }

    fprintf(saida, "\n=== BUSCA EM PROFUNDIDADE (ID 0) ===\n");
    DFS(&grafo, 0, saida);
    LimparVisitados(&grafo);

    fprintf(saida, "\n=== BUSCA EM LARGURA (ID 0) ===\n");
    BFS(&grafo, 0, saida);
    LimparVisitados(&grafo);

    if (grafo.total_antenas > 2) {
        fprintf(saida, "\n=== CAMINHOS ENTRE ID 0 E ID 2 ===\n");
        EncontrarTodosCaminhos(&grafo, 0, 2, saida);
    } else {
        fprintf(saida, "\n=== NÃO HÁ ANTENAS SUFICIENTES PARA MOSTRAR CAMINHOS ===\n");
    }

    fprintf(saida, "\n=== INTERSECOES ENTRE FREQUENCIAS ===\n");
    bool encontrou_intersecoes = false;
    for (int i = 0; i < grafo.total_antenas; i++) {
        for (int j = i + 1; j < grafo.total_antenas; j++) {
            if (grafo.antenas[i].frequencia != grafo.antenas[j].frequencia) {
                fprintf(saida, "\nIntersecoes entre %c e %c:\n", 
                       grafo.antenas[i].frequencia, 
                       grafo.antenas[j].frequencia);
                ListarIntersecoes(&grafo, grafo.antenas[i].frequencia, 
                                grafo.antenas[j].frequencia, saida);
                encontrou_intersecoes = true;
            }
        }
    }
    if (!encontrou_intersecoes) {
        fprintf(saida, "Nenhuma interseção encontrada entre frequências diferentes.\n");
    }

    fprintf(saida, "\n=== EFEITOS NEFASTOS ===\n");
    CalcularEfeitosNefastos(&grafo, saida);
    
    fclose(saida);
}

// Liberta a memória do grafo
void LibertarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->total_antenas; i++) {
        Ligacao* atual = grafo->antenas[i].conexoes;
        while (atual) {
            Ligacao* temp = atual;
            atual = atual->proxima;
            free(temp);
        }
    }
}