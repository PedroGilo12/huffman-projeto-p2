/**
 * @file input_process.c
 *
 * @brief Responsável pelos procedimentos de descompactação do arquivo.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde (rfa@ic.ufal.br)
 */

#include "extract_process.h"

#include "common.h"

int get_header_from_file(unsigned char *compressed_file_name, file_header_t **file_header)
{
    /* Abre o arquivo comprimido em modo binário para leitura ("rb"). */
    FILE *compressed_file = fopen(compressed_file_name, "rb");

    if (compressed_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    /* Variáveis de controle para os tamanhos presentes no cabeçalho*/
    unsigned int trash_size = 0;
    unsigned int tree_size  = 0;

    /* full_sizes armazenará os dois bytes do cabeçalho responsáveis pelos tamanhos. */
    unsigned char full_sizes[2];

    /* Lê os dois bytes do cabeçalho. */
    fread(full_sizes, 2, 1, compressed_file);

    /* Recupera o tamanho do lixo e o tamanho da árvore. */
    /*
     * A operação & 0xFF serve para zerar todos os outros bits que não façam parte do
     * primeiro byte do unsigned char
     */
    trash_size = trash_size | ((full_sizes[0] & 0xFF) >> 5);

    /* Recupera o tamanho da árvore. */
    tree_size = (tree_size | full_sizes[0]); /* Pega os valores no primeiro byte. */
    tree_size = (tree_size << 8);            /* Move eles para a posição certa. */
    tree_size = tree_size & 0x1FFF;          /* Zera os bits de lixo. */
    tree_size = tree_size | full_sizes[1];   /* Insere os bits do segundo byte. */

    /* Aloca memória provisóriamente para os bytes da árvore. */
    unsigned char *byte = (unsigned char *) malloc(sizeof(unsigned char));

    /* Aloca memória para a string em pré ordem, considerando o pior caso onde existem 2
     * caracteres de escape. */
    unsigned char *pre_order_tree =
        (unsigned char *) malloc((tree_size + 2) * sizeof(unsigned char));

    unsigned int header_end = tree_size;

    printf("Header from file: ");
    for (int index = 0; index < header_end; index++) {
        fread(byte, 1, 1, compressed_file);

        /* Se for um caracgter de escape aumenta em +1 o final do laço. */
        if (*byte == '\\') {
            header_end += 1;
        }

        pre_order_tree[index] = *byte;

        printf("%c", *byte);
    }
    printf("\n");

    /* Atribui os valores do cabeçalho. */
    (*file_header)->tree_size     = tree_size;
    (*file_header)->trash_size    = trash_size;
    (*file_header)->preorder_tree = pre_order_tree;

    /* Libera a memória e fecha o arquivo. */
    free(byte);
    fclose(compressed_file);

    return 0;
}

int extract_file(unsigned char *compressed_file_name, unsigned char *extracted_file_name,
                 huffman_tree_t *huffman_tree, int qtd_caracter_scape)
{
    FILE *compressed_file = fopen(compressed_file_name, "rb");
    FILE *extracted_file  = fopen(extracted_file_name, "w+b");

    if (compressed_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    linked_list_t *current   = huffman_tree->linkedList;
    unsigned long binary_way = 0; /* Resultado do caminho até uma folha. */
    unsigned int shift_bit   = 0; /* Bit atual lido no arquivo. */
    unsigned long end_file   = 0; /* Posição inicial do lixo. */

    /* Conseguir a posição do ínicio do lixo. */
    fseek(compressed_file, 0, SEEK_END);
    end_file = ftell(compressed_file);

    /* Posiciona o ponteiro no começo do arquivo menos o cabeçalho. */
    fseek(compressed_file, huffman_tree->tree_size + 2, SEEK_SET);

    while (1) {
        /* Lê um byte do arquivo. */
        unsigned char byte;
        if (fread(&byte, 1, 1, compressed_file) != 1) {
            break;
        }

        /* Laço para varrer todos os bits daquele byte do arquivo. */
        for (int shift_cache = 7; shift_cache >= 0; shift_cache--) {
            /* Se o nó autla for uma folha e não for NULL. */
            if (current != NULL && current->left == NULL && current->right == NULL) {
                /* O byte alvo é o byte daquela folha. */
                unsigned char target_byte =
                    (unsigned char) ((byte_frequency_t *) (current->data))->byte;

                /* Escreve o byte no arquivo. */
                size_t write =
                    fwrite(&target_byte, sizeof(target_byte), 1, extracted_file);

                /* Caso não seja possível gravar o byte no arquivo. */
                if (write != 1) {
                    fclose(extracted_file);
                    return ERR_FILE_WRITE;
                }

                /* Reinicia o caminho até a folha. */
                binary_way = 0;

                /* Recupera o ponteiro atual dentro do arquivo. */
                unsigned long current_p_file = ftell(compressed_file);

                /* Volta para o nó root. */
                current = huffman_tree->linkedList;

                /* Se chegar no final do arquivo pare o laço. */
                if (current_p_file >= end_file || current == NULL) {
                    break;
                }
            }

            /* Isola 1 bit do byte lido. */
            shift_bit = (byte >> shift_cache) & 0x1;

            /* Verifica se deve seguir para a direita ou para esquerda, se for para
             * esquerda adiciona 0 ao caminho binário, se for para direita adiciona 1. */
            if (shift_bit == 0) {
                binary_way = (binary_way << 1) | 0;
                current    = current->left;
            }

            if (shift_bit == 1) {
                binary_way = (binary_way << 1) | 1;
                current    = current->right;
            }
        }
    }

    /* Fecha ambos os arquivos. */
    fclose(compressed_file);
    fclose(extracted_file);

    return 0;
}


linked_list_t *create_tree_from_preorder(unsigned char *string, int *index,
                                         int *qtd_scape_caracter)
{
    /* Se o item for um * ele vai criar um novo nó sempre antes de continuar a árvore. */
    if (string[*index] == '*') {
        linked_list_t *node = new_node(&string[*index]);
        (*index)++;
        node->left  = (linked_list_t *) create_tree_from_preorder(string, index,
                                                                  qtd_scape_caracter);
        node->right = (linked_list_t *) create_tree_from_preorder(string, index,
                                                                  qtd_scape_caracter);
        return node;
    } else {
        /* Se o item for uma / ele vai pular ele e continuar a analisar os outros itens.
         */
        if (string[*index] == '\\') {
            (*qtd_scape_caracter)++;
            (*index)++;
        }

        linked_list_t *node = new_node(&string[*index]);
        (*index)++;

        return node;
    }
}
