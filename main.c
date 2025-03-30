/**
 * @file main.c
 * @brief Programa principal com output em ficheiro
 * @details Escreve resultados no ficheiro resultado.txt sem output no terminal
 */

 #include "funcoes.h"
 #include <stdio.h>
 
 int main() {
     FILE *output = fopen("resultado.txt", "w");
     if (!output) return 1;
 
     // 1. Carregar antenas
     antena* antenas = CarregarAntenasDoFicheiro("antenas.txt");
     if (!antenas) {
         fprintf(output, "Erro ao carregar antenas!\n");
         fclose(output);
         return 1;
     }
 
     // 2. Calcular efeitos
     nefasto* efeitos = CalcularEfeitosNefastos(antenas);
     
     // 3. Escrever resultados no ficheiro
     fprintf(output, "=== ANTENAS ===\n");
     for (antena* a = antenas; a; a = a->prox) {
         fprintf(output, "Freq: %c @ (%d,%d)\n", a->frequencia, a->coluna, a->linha);
     }
 
     fprintf(output, "\n=== INTERFERÊNCIAS ===\n");
     for (nefasto* e = efeitos; e; e = e->prox) {
         fprintf(output, "Posição: (%d,%d)\n", e->coluna, e->linha);
     }
 
     // 4. Libertar memória e fechar ficheiro
     LibertarAntenas(antenas);
     LibertarEfeitosNefastos(efeitos);
     fclose(output);
 
     return 0;
 }