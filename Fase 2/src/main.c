/**
 * @file main.c
 * @brief Programa principal para análise de rede de antenas
 * @details Carrega os dados de antenas de um ficheiro e exporta para um ficheiro resultado.txt
 */

#include "funcoes.h"
/**
 * @brief Função principal do programa
 * @return 0 se executado com sucesso, 1 em caso de erro
 */
int main() {
    printf("Iniciando processamento...\n");
    Grafo grafo = CarregarAntenasDoFicheiro("antenas.txt");
    
    if (grafo.total_antenas == 0) {
        printf("Nenhuma antena carregada. Verifique o arquivo de entrada.\n");
        return 1;
    }
    
    printf("Exportando resultados para resultado.txt...\n");
    ExportarResultados(grafo, "resultado.txt");
    
    LibertarGrafo(&grafo);
    printf("Processamento concluído com sucesso!\n");
    return 0;
}