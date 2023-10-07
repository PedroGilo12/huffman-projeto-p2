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

#ifndef GZIP_V1_EXTRACT_PROCESS_H
#define GZIP_V1_EXTRACT_PROCESS_H
#include "common.h"

/**
 * @brief Estrutura do cabeçalho.
 */
typedef struct file_header {
    unsigned long trash_size;     /**< Tamanho do lixo. */
    unsigned long tree_size;      /**< Tamanho da árvore. */
    unsigned char *preorder_tree; /**< String da árvore em pré ordem. */
} file_header_t;

/**
 * @brief Recupera o cabeçalho do arquivo comprimido.
 *
 * @param compressed_file_name Arquivo comprimido.
 * @param file_header [out] Ponteiro para a estrutura que vai receber o cabeçalho.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
int get_header_from_file(unsigned char *compressed_file_name,
                         file_header_t **file_header);

/**
 * @brief Recebe a string já em pré-ordem e transforma ela em uma árvore binária
 * utilizando recursão.
 *
 * @param string String que armazena os itens em pré-ordem.
 * @param index Utilizado para navegar pelos itens.
 * @param qtd_scape_caracter [out] Quantidade de caracteres de escape no cabeçalho.
 */
linked_list_t *create_tree_from_preorder(unsigned char *string, int *index,
                                         int *qtd_scape_caracter);

/**
 * @brief   Realiza o processo de extração do arquivo, ou seja, recupera bit a bit cada
 *          byte do arquivo e faz sua devida correlação com o dicionário.
 * @param compressed_file_name Nome do arquivo comprimido.
 * @param extracted_file_name Nome do arquivo extraído.
 * @param huffman_tree Estrutura da árvore de huffman.
 * @param qtd_caracter_scape Quantidade de caracteres de escape no cabeçalho.
 *
 * @return 0 em caso de sucesso ou inteiro negativo em caso de falha.
 */
int extract_file(unsigned char *compressed_file_name, unsigned char *extracted_file_name,
                 huffman_tree_t *huffman_tree, int qtd_caracter_scape);

#endif  // GZIP_V1_EXTRACT_PROCESS_H
