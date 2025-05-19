/**
 * @file main.c
 * @brief Programa principal para análise de rede de antenas
 * @details Este programa carrega uma rede de antenas a partir de um ficheiro,
 *          realiza várias análises e exporta os resultados para outro ficheiro.
 */

#include "funcoes.h"

/**
 * @brief Função principal do programa
 * @return 0 se o programa foi executado com sucesso, 1 caso ocorra algum erro
 * 
 * @note O programa espera um ficheiro de entrada chamado "antenas.txt"
 *       e gera um ficheiro de saída chamado "resultado.txt"
 */
int main() {
    // Carregar a rede de antenas do ficheiro
    Grafo grafo = CarregarAntenasDoFicheiro("antenas.txt");
    
    // Verificar se o carregamento foi bem sucedido
    if (grafo.total_antenas == 0) {
        fprintf(stderr, "Erro: Nenhuma antena foi carregada.\n");
        return 1;
    }
    
    // Exportar os resultados das análises
    if (!ExportarResultados(grafo, "resultado.txt")) {
        fprintf(stderr, "Erro ao exportar resultados.\n");
        LibertarGrafo(&grafo);
        return 1;
    }
    
    // Libertar a memória alocada
    LibertarGrafo(&grafo);
    return 0;
}