#include "output_process.h"

#include <common.h>

int get_header_from_file(unsigned char *compressed_file_name, file_header_t **file_header)
{
    FILE *compressed_file = fopen(compressed_file_name, "rb");

    unsigned int trash_size = 0;
    unsigned int tree_size  = 0;

    unsigned char *full_sizes = (unsigned char *) malloc(2 * sizeof(unsigned char));

    fread(full_sizes, 2, 1, compressed_file);

    trash_size = trash_size | ((full_sizes[0] & 0xFF) >> 5);
    tree_size  = (tree_size | full_sizes[0]);
    tree_size = (tree_size << 8);
    tree_size = tree_size & 0x1FFF;
    tree_size = tree_size | full_sizes[1];

    free(full_sizes);

    unsigned char *byte = (unsigned char *) malloc(sizeof(unsigned char));
    unsigned char *pre_order_tree =
        (unsigned char *) malloc((tree_size + 2) * sizeof(unsigned char));

    unsigned int header_end = tree_size;

    printf("Header from file: ");
    for (int index = 0; index < header_end; index++) {
        fread(byte, 1, 1, compressed_file);

        if (*byte == '\\') {
            header_end += 1;
        }

        pre_order_tree[index] = *byte;

        printf("%c", *byte);
    }

    printf("\n");
    (*file_header)->tree_size     = tree_size;
    (*file_header)->trash_size    = trash_size;
    (*file_header)->preorder_tree = pre_order_tree;

    fclose(compressed_file);

    return 0;
}

int extract_file(unsigned char *compressed_file_name, unsigned char *extracted_file_name,
                 huffman_tree_t *huffman_tree, int qtd_caracter_scape)
{
    FILE *compressed_file = fopen(compressed_file_name, "rb");
    FILE *extracted_file  = fopen(extracted_file_name, "w+b");

    if (!compressed_file) {
        return ERR_FILE_NOT_FOUND;
    }

    linked_list_t *current      = huffman_tree->linkedList;
    unsigned long binary_word   = 0;
    unsigned int shift_bit      = 0;
    unsigned int target_eq_byte = 0;
    unsigned long end_file      = 0;

    /* Conseguir a posição do ínicio do lixo. */
    fseek(compressed_file, 0, SEEK_END);
    end_file = ftell(compressed_file);

    fseek(compressed_file, huffman_tree->tree_size + 2 + qtd_caracter_scape, SEEK_SET);

    while (1) {
        unsigned char byte;

        if (fread(&byte, 1, 1, compressed_file) != 1) {
            break;
        }

        for (int shift_cache = 7; shift_cache >= 0; shift_cache--) {
            if (current != NULL && current->left == NULL && current->right == NULL) {
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

                binary_word                  = 0;
                unsigned long current_p_file = ftell(compressed_file);

                current = huffman_tree->linkedList;

                if (current_p_file >= end_file) {
                    break;
                }
            }

            if(current == NULL) {
                break;
            }

            shift_bit = (byte >> shift_cache) & 0x1;

            if (shift_bit == 0) {
                binary_word = (binary_word << 1) | 0;
                current     = current->left;
            }

            if (shift_bit == 1) {
                binary_word = (binary_word << 1) | 1;
                current     = current->right;
            }
        }
    }

    fclose(compressed_file);
    return 0;  // Ou outro código de sucesso, dependendo do seu sistema.
}


linked_list_t *create_tree_from_preorder(unsigned char *string, int *index, int *qtd_scape_caracter)
{
    /* Se o item for um * ele vai criar um novo nó sempre antes de continuar a árvore. */
    if (string[*index] == '*') {
        linked_list_t *node = new_node(&string[*index]);
        (*index)++;
        node->left  = (linked_list_t *) create_tree_from_preorder(string, index, qtd_scape_caracter);
        node->right = (linked_list_t *) create_tree_from_preorder(string, index, qtd_scape_caracter);
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
