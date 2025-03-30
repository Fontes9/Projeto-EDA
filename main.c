#include "funcoes.h"
#include <stdio.h>

int main() {
    // Abre o ficheiro de saída
    FILE *output = fopen("Resultado.txt", "w");
    if (!output) {
        printf("Erro ao criar o ficheiro Resultado.txt!\n");
        return 1;
    }

    // Carrega as antenas
    antena* lista_antenas = CarregarAntenasDoFicheiro("antenas.txt");
    if (!lista_antenas) {
        fprintf(output, "Erro ao carregar as antenas!\n");
        fclose(output);
        return 1;
    }

    // Calcula os efeitos
    nefasto* lista_efeitos = CalcularEfeitosNefastos(lista_antenas);

    // Escreve os resultados no ficheiro
    fprintf(output, "Lista de Antenas (coluna,linha):\n");
    for (antena* a = lista_antenas; a != NULL; a = a->prox) {
        fprintf(output, "Freq: %c @ (%d,%d)\n", a->frequencia, a->coluna, a->linha);
    }

    fprintf(output, "\nEfeitos Nefastos:\n");
    for (nefasto* e = lista_efeitos; e != NULL; e = e->prox) {
        fprintf(output, "Posição: (%d,%d)\n", e->coluna, e->linha);
    }

    // Fecha o ficheiro e liberta a memória
    fclose(output);
    LibertarAntenas(lista_antenas);
    LibertarEfeitosNefastos(lista_efeitos);

    return 0;
}