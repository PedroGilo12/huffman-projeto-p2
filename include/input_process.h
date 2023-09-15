/**
 * @file input_process.c
 *
 * @brief Responsável por receber e tratar o arquivo de entrada.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde ()
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
int process_input_file_as_byte_frequency(const char* file_name,
                                         linked_list_t** linked_list);

#endif  // GZIP_V1_INPUT_PROCESS_H
