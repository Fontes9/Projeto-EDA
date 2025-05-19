/**
 * @file funcoes.h
 * @brief Cabeçalho para gestão de antenas e efeitos nefastos
 * @details Contém as estruturas de dados e protótipos das funções principais
 * @author Tiago Fontes
 */

 #ifndef FUNCOES_H
 #define FUNCOES_H
 
 #define MAX_LINHAS 12    ///< Número máximo de linhas na grelha
 #define MAX_COLUNAS 12   ///< Número máximo de colunas na grelha
 
 /**
  * @brief Estrutura de uma antena
  */
 typedef struct Antena {
     char frequencia;     ///< Frequência (ex: 'A', 'B', 'C')
     int coluna;         ///< Posição X (0 a MAX_COLUNAS-1)
     int linha;          ///< Posição Y (0 a MAX_LINHAS-1)
     struct Antena *prox; ///< Apontador para a próxima antena
 } antena;
 
 /**
  * @brief Estrutura para efeitos de interferência
  */
 typedef struct EfeitoNefasto {
     int coluna;                    ///< Coluna afetada
     int linha;                     ///< Linha afetada
     struct EfeitoNefasto *prox;    ///< Próximo efeito na lista
 } nefasto;
 
 // ------------------------------------
 // Protótipos das funções
 // ------------------------------------
 
 /**
  * @brief Cria uma nova antena
  * @param coluna Posição horizontal (começa em 0)
  * @param linha Posição vertical (começa em 0)
  * @param freq Frequência (carácter único)
  * @return Apontador para antena criada ou NULL se erro
  */
 antena* CriarAntena(int coluna, int linha, char freq);
 
 /**
  * @brief Carrega antenas de um ficheiro de texto
  * @param nome_ficheiro Caminho para o ficheiro
  * @return Lista ligada de antenas ou NULL se erro
  * @note Formato esperado:
  *       - 1ª linha: "total_linhas total_colunas"
  *       - Linhas seguintes: grelha com antenas (ex: "A..B\n.C.D")
  */
 antena* CarregarAntenasDoFicheiro(const char* nome_ficheiro);
 
 /**
  * @brief Calcula posições com interferência
  * @param lista Lista de antenas a analisar
  * @return Lista de efeitos nefastos ou NULL se nenhum encontrado
  */
 nefasto* CalcularEfeitosNefastos(antena* lista);
 
 /**
  * @brief Liberta memória de todas as antenas
  * @param lista Lista a ser libertada
  */
 void LibertarAntenas(antena* lista);
 
 /**
  * @brief Liberta memória dos efeitos nefastos
  * @param lista Lista a ser libertada
  */
 void LibertarEfeitosNefastos(nefasto* lista);
 
 #endif