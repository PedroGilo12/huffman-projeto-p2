/**
 * @file common.h
 *
 * @brief Funções utilizadas em todos os arquivos do programa.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde (rfa@ic.ufal.br)
 */

#ifndef GZIP_V1_COMMON_H
#define GZIP_V1_COMMON_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Se DEBUG_MODE == 1 então retornos de debug estarão habilitados.
 */
#define DEBUG_MODE 1

/**
 * @brief Enumeração para sinais de erro.
 */
typedef enum common_err {
    ERR_NONE = 0,       /**< Nenhum erro ocorreu. */
    ERR_NULL_POINTER,   /**< Ponteiro para NULL não permitido. */
    ERR_FILE_NOT_FOUND, /**< Arquivo não encontrado. */
    ERR_FILE_WRITE,     /**< Não foi possível escrever no arquivo. */
    ERR_ALLOC_MEM,      /**< Falha ao alocar memória. */
} common_err_t;

/**
 * @brief Estrutura responsável por armazenar um byte e sua frequência.
 */
typedef struct byte_frequency {
    char byte;               /**< Byte armazenado*/
    unsigned long frequency; /**< Frequência do byte armazenado */
} byte_frequency_t;

/**
 * @brief Estrutura genérica para uma lista encadeada com "huffman nodes".
 */
typedef struct linked_list {
    byte_frequency_t *data;    /**< Endereço do dado armazenado no nó */
    struct linked_list *next;  /**< Endereço do próximo nó*/
    struct linked_list *right; /**< Endereço do filho à direita do nó */
    struct linked_list *left;  /**< Endereço do filho à esquerda do nó */

    // TODO: Verificar implementação de um array de ponteiros para funções básicas da
    // lista
    void (*func)(struct linked_list);
} linked_list_t;

/**
 * @brief Estrutura para árvore de Huffman.
 */
typedef struct huffman_tree {
    linked_list_t * linkedList;  /**< Árvore de huffman */
    char * preorder;             /**< Árvore de huffman em pré-ordem */
    unsigned int tree_size;      /**< Tamanho da árvore */
    unsigned int trash_size;     /**< Tamanho do lixo */
    unsigned long ** dictionary; /**< Dicionário */
} huffman_tree_t;

/**
 * @brief Cria uma lista encadeada genérica vazia.
 *
 * @warning Uma linked_list vazia possui todos os seus campos iguais a NULL.
 * @return Endereço do primeiro nó da lista encadeada vazia.
 */
linked_list_t *create_empty_linked_list();

/**
 * @brief Insere um nó na cabeça da lista encadeada.
 *
 * @param head [in:out] endereço da cabeça da lista encadeada.
 * @param byte [in] endereço do dado a ser inserido na lista.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
int insert_byte_frequency_at_beginning(linked_list_t **head, const char *byte, unsigned long frequency);

/**
 * @brief Checa se já existe um nó na lista que contem aquele dado.
 *
 * @param linked_list [in] Endereço da lista encadeada.
 * @param target_char [in] Endereço do byte que será procurado.
 * @return Endereço do nó existente ou NULL em caso de nó não existente.
 */
linked_list_t *exist_byte_in_linked_list(linked_list_t *linked_list,
                                         const char *target_char);

/**
 * @brief Ordena a lista encadeada com base na frequência.
 *
 * @param linked_list [in:out] Endereço do ponteiro para a lista encadada.
 * @return 0 em caso de sucesso.
 */
int sort_linked_list_by_frequency(linked_list_t **linked_list);

/**
 * @brief Retorna no terminal as informações de frequência de cada byte.
 *
 * @param linked_list [in] Lista encadeada a ser retornada no terminal.
 */
void print_linked_list_byte_frequency(linked_list_t *linked_list);

/**
 * @brief Insere um nó ordenado na lista.
 *
 * @param linked_list [in:out] Endereço do ponteiro para a lista encadeada.
 * @param new_node [in] Novo nó a ser inserido na lista.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
int insert_ordered_in_linked_list(linked_list_t **linked_list, linked_list_t *new_node);

/**
 * @brief Encontra a profundidade de um nó na árvore de Huffman.
 *
 * @param linked_list [in] Ponteiro para a lista encadeada da árvore de Huffman.
 * @param target [in] Nó desejado.
 * @param depth Profundidade inicial, default: 0;
 * @return Profundidade do nó.
 */
unsigned long find_depth_in_huffman_tree(linked_list_t *linked_list, void *target,
                                         unsigned long depth);

/**
 * @brief Escreve no terminal bit a bit de um byte.
 * @param byte Byte que será escrito no terminal.
 */
void printf_bit_by_bit(char byte);

/**
 * @brief Conta os nós de uma lista encadeada.
 * @param head [in] Ponteiro para o começo da lista.
 * @return Quantidade de nós na lista.
 */
int count_nodes(linked_list_t *head);

#endif  // GZIP_V1_COMMON_H
