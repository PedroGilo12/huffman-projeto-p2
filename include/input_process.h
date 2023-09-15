//
// Created by Pedro on 14/09/2023.
//

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
int process_input_file(const char* file_name, linked_list_t** linked_list);

#endif  // GZIP_V1_INPUT_PROCESS_H
