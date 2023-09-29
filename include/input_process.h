/**
 * @file input_process.c
 *
 * @brief Responsável por receber e tratar o arquivo de entrada.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde (rfa@ic.ufal.br)
 */

#ifndef GZIP_V1_INPUT_PROCESS_H
#define GZIP_V1_INPUT_PROCESS_H
#include <common.h>

#define STATIC_RESET NULL, NULL, NULL, -1

/**
 * @brief   Conta o numero de caracteres de um arquivo e armazena em uma lista encadeada
 *          onde cada nó contém um caracter presente no arquivo e sua frequência.
 *
 * @param file_name Nome do arquivo que será processado.
 * @param linked_list [out] Lista encadeada com os bytes presentes no arquivo.
 * @return 0 em caso de sucesso ou inteiro negativo em caso de falha.
 */
int process_input_file_as_byte_frequency(const unsigned char *file_name,
                                         linked_list_t **linked_list);

/**
 * @brief Cria uma árvore de Huffman a partir de uma lista encadeada.
 * @param linked_list [in] Lista encadeada.
 * @return Árvore de Huffman.
 */
huffman_tree_t *make_huffman_tree(linked_list_t **linked_list);

/**
 * @brief Cria um dicionário e uma string em pré ordem com base na árvore de Huffman.
 *
 * @param linked_list [in] Lista encadeada da árvore de Huffman.
 * @param dictionary [out] Dicionário gerado a partir da árvore.
 * @param pre_order_tree [out] String da árvore em pré ordem.
 * @param binary_word Inicio do dicionário, padrão: 0;
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
int make_preorder_dictionary(linked_list_t **linked_list, unsigned long ***dictionary,
                             unsigned char **pre_order_tree, int binary_word);

/**
 * @brief Comprime um arquivo de entrada com base em uma árvore de Huffman.
 * @param input_file_name Nome do arquivo de entrada.
 * @param output_file_name Nome do arquivo de saída.
 * @param huffman_tree Árvore de Huffman.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
int compress_file(unsigned char *input_file_name, unsigned char *output_file_name,
                  huffman_tree_t *huffman_tree);

#endif  // GZIP_V1_INPUT_PROCESS_H
