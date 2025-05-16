#include "funcoes.h"

int main() {
    Grafo grafo = CarregarAntenasDoFicheiro("antenas.txt");
    ExportarResultados(grafo, "resultado.txt");
    LibertarGrafo(&grafo);
    return 0;
}