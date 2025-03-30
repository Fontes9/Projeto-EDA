/**
 * @file main.c
 * @brief Programa principal
 * @author Tiago Fontes
 */

 #include "funcoes.h"
 #include <stdio.h>
 
 int main() {
     // 1. Carregar as antenas
     antena* antenas = CarregarAntenasDoFicheiro("antenas.txt");
     if (!antenas) {
         printf("Erro ao carregar antenas!\n");
         return 1;
     }
 
     // 2. Calcular os efeitos
     nefasto* efeitos = CalcularEfeitosNefastos(antenas);
     
     // 3. Escrever os resultados
     FILE *output = fopen("resultado.txt", "w");
     if (!output) {
         printf("Erro ao criar ficheiro de saída!\n");
         LibertarAntenas(antenas);
         return 1;
     }
 
     fprintf(output, "=== ANTENAS ===\n");
     for (antena* a = antenas; a; a = a->prox) {
         fprintf(output, "Freq: %c @ (%d,%d)\n", a->frequencia, a->coluna, a->linha);
     }
 
     fprintf(output, "\n=== INTERFERÊNCIAS ===\n");
     for (nefasto* e = efeitos; e; e = e->prox) {
         fprintf(output, "Posição: (%d,%d)\n", e->coluna, e->linha);
     }
 
     // 4. Libertar a memória
     LibertarAntenas(antenas);
     LibertarEfeitosNefastos(efeitos);
     fclose(output);
 
     return 0;
 }