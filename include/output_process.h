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

typedef struct file_header {
    unsigned long trash_size;
    unsigned long tree_size;
    char * preorder_tree;
} file_header_t;

/**
 * @brief
 * @param compressed_file_name
 * @param file_header
 * @return
 */
int get_header_from_file(char * compressed_file_name, file_header_t ** file_header);

/**
 * @brief Recebe a string já em pré-ordem e transforma ela em uma árvore binária utilizando recursão.
 *
 * @param string String que armazena os itens em pré-ordem.
 * @param index Utilizado para navegar pelos itens.
 *
 */
linked_list_t* create_tree_from_preorder(char *string, int *index);

/**
 * @brief
 * @param compressed_file_name
 * @param extracted_file_name
 * @param huffman_tree
 * @return
 */
int extract_file(char *compressed_file_name, char *extracted_file_name,
                 huffman_tree_t *huffman_tree);

#endif //GZIP_V1_OUTPUT_PROCESS_H
