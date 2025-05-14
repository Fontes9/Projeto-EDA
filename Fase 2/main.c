/**
 * @file main.c
 * @brief Programa principal para a Fase 2 (Grafos)
 * @author Tiago Fontes
 */

#include "funcoes.h"
#include <stdio.h>

int main() {
    // 1. Carregar o grafo
    Grafo grafo = CarregarGrafoDoFicheiro("antenas.txt");
    if (grafo.num_vertices == 0) {
        printf("Erro ao carregar grafo!\n");
        return 1;
    }
    
    // 2. Informações básicas
    printf("=== INFORMACOES DO GRAFO ===\n");
    ImprimirGrafo(grafo);
    ImprimirMatrizAdjacencia(grafo);
    
    // 3. Operações de busca
    printf("\n=== BUSCA EM PROFUNDIDADE (V0) ===\n");
    bool visitados[MAX_VERTICES] = {false};
    BuscaProfundidade(grafo, 0, visitados);
    
    printf("\n=== BUSCA EM LARGURA (V0) ===\n");
    BuscaLargura(grafo, 0);
    
    // 4. Caminhos entre vértices
    printf("\n=== CAMINHOS ENTRE VERTICES ===\n");
    int caminho[MAX_VERTICES];
    bool visitados_caminho[MAX_VERTICES] = {false};
    
    if (grafo.num_vertices > 2) {
        printf("Caminhos entre V0 e V2:\n");
        EncontrarTodosCaminhos(grafo, 0, 2, caminho, visitados_caminho, 0);
    }
    
    // 5. Intersecções
    printf("\n=== INTERSECOES ENTRE FREQUENCIAS ===\n");
    ListarIntersecoes(grafo, 'A', 'B');
    
    // 6. Cálculo de interferências
    printf("\n=== PONTOS DE INTERFERENCIA ===\n");
    nefasto* efeitos = CalcularEfeitosNefastosGrafo(grafo);
    for (nefasto* e = efeitos; e; e = e->prox) {
        printf("(%d,%d) - Frequencia: %c\n", e->coluna, e->linha, e->frequencia);
    }
    
    // 7. Libertar memória
    LibertarEfeitosNefastos(efeitos);
    
    return 0;
}