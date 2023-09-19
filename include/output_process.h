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

#ifndef GZIP_V1_OUTPUT_PROCESS_H
#define GZIP_V1_OUTPUT_PROCESS_H
#include <common.h>

/**
 * @brief Recebe a string já em pré-ordem e transforma ela em uma árvore binária utilizando recursão.
 *
 * @param string String que armazena os itens em pré-ordem.
 * @param index Utilizado para navegar pelos itens.
 *
 */

linked_list_t* create_tree_from_preorder(const char string[], int *index);

#endif //GZIP_V1_OUTPUT_PROCESS_H
