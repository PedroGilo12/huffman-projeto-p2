#include "output_process.h"

#include <common.h>

int get_header_from_file(char *compressed_file_name, file_header_t **file_header)
{
    FILE *compressed_file = fopen(compressed_file_name, "rb");

    unsigned int trash_size = 0;
    unsigned int tree_size  = 0;

    char *full_sizes = (char *) malloc(2 * sizeof(char));

    fread(full_sizes, 2, 1, compressed_file);

    trash_size = trash_size | ((full_sizes[0] & 0xFF) >> 5);
    tree_size  = (tree_size | ((full_sizes[0] & 0x1F)) << 5) | (full_sizes[1] & 0xFF);

    free(full_sizes);

    char *byte           = (char *) malloc(sizeof(char));
    char *pre_order_tree = (char *) malloc((tree_size + 2) * sizeof(char));

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

int extract_file(char *compressed_file_name, char *extracted_file_name,
                 huffman_tree_t *huffman_tree)
{
    FILE *compressed_file = fopen(compressed_file_name, "rb");

    if (fseek(compressed_file, huffman_tree->tree_size + 2, SEEK_SET) != 0)
    {
        perror("Erro ao fazer fseek");
        fclose(compressed_file);
        return 1;
    }
    char *byte = malloc(sizeof(char));

    fread(byte, 1, 1, compressed_file);

    printf("primeiro byte pos header: ");
    printf_bit_by_bit(*byte);

    //
    //    while (1) {
    //        /* Alocação de memória para armazenar provisóriamente o byte lido no arquivo
    //        */ char *byte = malloc(sizeof(char));
    //
    //        if (fread(byte, 1, 1, compressed_file) != 1) {
    //            /* Libera a memória reservada para byte antes de sair do loop */
    //            free(byte);
    //            break;
    //        }
    //
    //
    //
    //        }
    //    }
    //
}

linked_list_t *create_tree_from_preorder(char *string, int *index)
{
    /* Se o item for um * ele vai criar um novo nó sempre antes de continuar a árvore. */
    if (string[*index] == '*') {
        linked_list_t *node = new_node(&string[*index]);
        (*index)++;
        node->left  = (linked_list_t *) create_tree_from_preorder(string, index);
        node->right = (linked_list_t *) create_tree_from_preorder(string, index);
        return node;
    } else {
        /* Se o item for uma / ele vai pular ele e continuar a analisar os outros itens.
         */
        if (string[*index] == '/') {
            (*index)++;
        }

        linked_list_t *node = new_node(&string[*index]);
        (*index)++;

        return node;
    }
}
