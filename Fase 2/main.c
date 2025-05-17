#include "funcoes.h"

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