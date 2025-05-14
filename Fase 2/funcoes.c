/**
 * @file funcoes.c
 * @brief Implementação das funções de gestão de grafos e interferências
 * @author Tiago Fontes
 */

#include "funcoes.h"

Grafo InicializarGrafo() {
    Grafo grafo;
    grafo.num_vertices = 0;
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            grafo.matriz_adj[i][j] = 0;
        }
    }
    
    return grafo;
}

Grafo CarregarGrafoDoFicheiro(const char* nome_ficheiro) {
    FILE* fp = fopen(nome_ficheiro, "r");
    Grafo grafo = InicializarGrafo();
    
    if (fp == NULL) {
        fprintf(stderr, "Erro ao abrir %s\n", nome_ficheiro);
        return grafo;
    }

    int total_linhas, total_colunas;
    if (fscanf(fp, "%d %d", &total_linhas, &total_colunas) != 2) {
        fclose(fp);
        fprintf(stderr, "Formato inválido\n");
        return grafo;
    }

    char linha[MAX_COLUNAS + 2];
    int y = 0;

    while (fgets(linha, sizeof(linha), fp) && y < total_linhas) {
        linha[strcspn(linha, "\n")] = '\0';
        for (int x = 0; x < total_colunas && x < (int)strlen(linha); x++) {
            if (linha[x] != '.' && linha[x] != ' ') {
                AdicionarVertice(&grafo, linha[x], x, y);
            }
        }
        y++;
    }
    
    fclose(fp);
    ConectarVerticesMesmaFrequencia(&grafo);
    return grafo;
}

bool AdicionarVertice(Grafo* grafo, char freq, int coluna, int linha) {
    if (grafo->num_vertices >= MAX_VERTICES) {
        return false;
    }
    
    Vertice novo;
    novo.frequencia = freq;
    novo.coluna = coluna;
    novo.linha = linha;
    novo.id = grafo->num_vertices;
    
    grafo->vertices[grafo->num_vertices] = novo;
    grafo->num_vertices++;
    
    return true;
}

void ConectarVerticesMesmaFrequencia(Grafo* grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        for (int j = i + 1; j < grafo->num_vertices; j++) {
            if (grafo->vertices[i].frequencia == grafo->vertices[j].frequencia) {
                grafo->matriz_adj[i][j] = 1;
                grafo->matriz_adj[j][i] = 1;
            }
        }
    }
}

void BuscaProfundidade(Grafo grafo, int id_origem, bool visitados[]) {
    visitados[id_origem] = true;
    printf("Antena %c @ (%d,%d)\n", 
           grafo.vertices[id_origem].frequencia,
           grafo.vertices[id_origem].coluna,
           grafo.vertices[id_origem].linha);
    
    for (int i = 0; i < grafo.num_vertices; i++) {
        if (grafo.matriz_adj[id_origem][i] && !visitados[i]) {
            BuscaProfundidade(grafo, i, visitados);
        }
    }
}

void BuscaLargura(Grafo grafo, int id_origem) {
    bool visitados[MAX_VERTICES] = {false};
    int fila[MAX_VERTICES];
    int inicio = 0, fim = 0;
    
    fila[fim++] = id_origem;
    visitados[id_origem] = true;
    
    while (inicio < fim) {
        int atual = fila[inicio++];
        printf("Antena %c @ (%d,%d)\n", 
               grafo.vertices[atual].frequencia,
               grafo.vertices[atual].coluna,
               grafo.vertices[atual].linha);
        
        for (int i = 0; i < grafo.num_vertices; i++) {
            if (grafo.matriz_adj[atual][i] && !visitados[i]) {
                fila[fim++] = i;
                visitados[i] = true;
            }
        }
    }
}

void EncontrarTodosCaminhos(Grafo grafo, int origem, int destino, int caminho[], bool visitados[], int pos) {
    caminho[pos] = origem;
    visitados[origem] = true;
    pos++;
    
    if (origem == destino) {
        printf("Caminho encontrado: ");
        for (int i = 0; i < pos; i++) {
            Vertice v = grafo.vertices[caminho[i]];
            printf("%c(%d,%d)", v.frequencia, v.coluna, v.linha);
            if (i < pos - 1) printf(" -> ");
        }
        printf("\n");
    } else {
        for (int i = 0; i < grafo.num_vertices; i++) {
            if (grafo.matriz_adj[origem][i] && !visitados[i]) {
                EncontrarTodosCaminhos(grafo, i, destino, caminho, visitados, pos);
            }
        }
    }
    
    visitados[origem] = false;
    pos--;
}

void ListarIntersecoes(Grafo grafo, char freqA, char freqB) {
    printf("Intersecoes entre antenas %c e %c:\n", freqA, freqB);
    
    for (int i = 0; i < grafo.num_vertices; i++) {
        if (grafo.vertices[i].frequencia == freqA) {
            for (int j = 0; j < grafo.num_vertices; j++) {
                if (grafo.vertices[j].frequencia == freqB) {
                    if (grafo.vertices[i].linha == grafo.vertices[j].linha || 
                        grafo.vertices[i].coluna == grafo.vertices[j].coluna) {
                        printf("Antena %c(%d,%d) e Antena %c(%d,%d)\n",
                               freqA, grafo.vertices[i].coluna, grafo.vertices[i].linha,
                               freqB, grafo.vertices[j].coluna, grafo.vertices[j].linha);
                    }
                }
            }
        }
    }
}

void ImprimirGrafo(Grafo grafo) {
    printf("Grafo com %d vertices:\n", grafo.num_vertices);
    for (int i = 0; i < grafo.num_vertices; i++) {
        Vertice v = grafo.vertices[i];
        printf("V%d: %c @ (%d,%d) -> ", v.id, v.frequencia, v.coluna, v.linha);
        
        bool primeiro = true;
        for (int j = 0; j < grafo.num_vertices; j++) {
            if (grafo.matriz_adj[i][j]) {
                if (!primeiro) printf(", ");
                printf("V%d", j);
                primeiro = false;
            }
        }
        printf("\n");
    }
}

int EncontrarIndiceVertice(Grafo grafo, int coluna, int linha) {
    for (int i = 0; i < grafo.num_vertices; i++) {
        if (grafo.vertices[i].coluna == coluna && grafo.vertices[i].linha == linha) {
            return i;
        }
    }
    return -1;
}

void ImprimirMatrizAdjacencia(Grafo grafo) {
    printf("Matriz de Adjacencia:\n   ");
    for (int i = 0; i < grafo.num_vertices; i++) {
        printf("V%d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < grafo.num_vertices; i++) {
        printf("V%d ", i);
        for (int j = 0; j < grafo.num_vertices; j++) {
            printf(" %d ", grafo.matriz_adj[i][j]);
        }
        printf("\n");
    }
}

nefasto* CalcularEfeitosNefastosGrafo(Grafo grafo) {
    nefasto* nefastos = NULL;
    
    for (int i = 0; i < grafo.num_vertices; i++) {
        for (int j = i + 1; j < grafo.num_vertices; j++) {
            if (grafo.matriz_adj[i][j]) {
                Vertice v1 = grafo.vertices[i];
                Vertice v2 = grafo.vertices[j];
                
                int diff_linha = v2.linha - v1.linha;
                int diff_coluna = v2.coluna - v1.coluna;
                
                // Direção v2 -> futuro
                int nefasto_linha = v2.linha + diff_linha;
                int nefasto_coluna = v2.coluna + diff_coluna;
                
                if (nefasto_linha >= 0 && nefasto_coluna >= 0 &&
                    nefasto_linha < MAX_LINHAS && nefasto_coluna < MAX_COLUNAS) {
                    nefasto* novo = (nefasto*)malloc(sizeof(nefasto));
                    if (novo) {
                        novo->coluna = nefasto_coluna;
                        novo->linha = nefasto_linha;
                        novo->frequencia = v1.frequencia;
                        novo->prox = nefastos;
                        nefastos = novo;
                    }
                }
                
                // Direção v1 -> passado
                nefasto_linha = v1.linha - diff_linha;
                nefasto_coluna = v1.coluna - diff_coluna;
                
                if (nefasto_linha >= 0 && nefasto_coluna >= 0 &&
                    nefasto_linha < MAX_LINHAS && nefasto_coluna < MAX_COLUNAS) {
                    nefasto* novo = (nefasto*)malloc(sizeof(nefasto));
                    if (novo) {
                        novo->coluna = nefasto_coluna;
                        novo->linha = nefasto_linha;
                        novo->frequencia = v1.frequencia;
                        novo->prox = nefastos;
                        nefastos = novo;
                    }
                }
            }
        }
    }
    return nefastos;
}

void LibertarEfeitosNefastos(nefasto* lista) {
    while (lista) {
        nefasto* temp = lista;
        lista = lista->prox;
        free(temp);
    }
}