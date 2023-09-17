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

/**
 * @brief   Conta o numero de caracteres de um arquivo e armazena em uma lista encadeada
 *          onde cada nó contém um caracter presente no arquivo e sua frequência.
 *
 * @param file_name Nome do arquivo que será processado.
 * @param linked_list [out] Lista encadeada com os bytes presentes no arquivo.
 * @return 0 em caso de sucesso ou inteiro negativo em caso de falha.
 */
int process_input_file_as_byte_frequency(const char *file_name,
                                         linked_list_t **linked_list);

/**
 * @brief
 * @param linked_list
 * @return
 */
huffman_tree_t *make_huffman_tree(linked_list_t **linked_list);

/**
 * @brief
 * @param linked_list
 * @param dictionary
 * @param pre_order_tree
 * @param binary_word
 * @return
 */
int make_dictionary(linked_list_t **linked_list, unsigned long ***dictionary,
                    char **pre_order_tree, unsigned int binary_word);

/**
 * @brief
 * @param input_file_name
 * @param output_file_name
 * @param huffman_tree
 * @return
 */
int compress_file(char *input_file_name, char *output_file_name,
                  huffman_tree_t *huffman_tree);

#endif  // GZIP_V1_INPUT_PROCESS_H
