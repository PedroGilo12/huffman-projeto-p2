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

#include <input_process.h>

int process_input_file_as_byte_frequency(const char *file_name,
                                         linked_list_t **linked_list)
{
    /* Verificação de ponteiro para garantir que os ponteiros são válidos */
    if ((file_name == NULL) || (linked_list == NULL)) {
        return ERR_NULL_POINTER;
    }

    /* Ponteiro para o arquivo no modo leitura binária "rb" */
    FILE *input_file = fopen(file_name, "rb");

    if (input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    while (1) {
        /* Alocação de memória para armazenar provisóriamente o byte lido no arquivo */
        char *byte = malloc(sizeof(char));

        if (fread(byte, 1, 1, input_file) != 1) {
            /* Libera a memória reservada para byte antes de sair do loop */
            free(byte);
            break;
        }

        /* Nó atual da lista encadeada */
        linked_list_t *current = exist_byte_in_linked_list(*linked_list, byte);

        if (current != NULL) {
            /* Se o nó atual já existir, apenas adicionar mais um a frequência */
            ((byte_frequency_t *) current->data)->frequency += 1;

#if DEBUG_MODE
            printf("Caracter já existe %c, frequência: %lu\n",
                   ((byte_frequency_t *) current->data)->byte,
                   ((byte_frequency_t *) current->data)->frequency);
#endif
        } else {
            /* Se o nó não existir, inserir o nó no começo da lista */
            insert_byte_frequency_at_beginning(linked_list, byte, 1);

#if DEBUG_MODE
            printf("Caracter inserido %c\n", byte[0]);
#endif
        }
    }

    fclose(input_file);
    return 0;
}

huffman_tree_t *make_huffman_tree(linked_list_t **linked_list)
{
    /* Alocam memoria para a arvore de */
    huffman_tree_t *huffman_tree = (huffman_tree_t *) malloc(sizeof(huffman_tree_t));

    int tree_size = count_nodes(*linked_list);
    unsigned long **dictionary =
        (unsigned long **) malloc(tree_size * sizeof(unsigned long *));

    if (*linked_list == NULL || (*linked_list)->next == NULL) {
        return NULL;
    }

    sort_linked_list_by_frequency(linked_list);

    while ((*linked_list)->next->data != NULL) {
        linked_list_t *current = *linked_list;
        linked_list_t *next    = current->next;

        byte_frequency_t *new_byte_frequency =
            (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

        if (new_byte_frequency == NULL) {
            return NULL;
        }

        unsigned long current_byte_frequency =
            ((byte_frequency_t *) current->data)->frequency;
        unsigned long next_byte_frequency = ((byte_frequency_t *) next->data)->frequency;

        new_byte_frequency->byte      = '*';
        new_byte_frequency->frequency = current_byte_frequency + next_byte_frequency;

        linked_list_t *new_node = (linked_list_t *) malloc(sizeof(linked_list_t));

        if (new_node == NULL) {
            return NULL;
        }

        new_node->data  = (byte_frequency_t *) new_byte_frequency;
        new_node->left  = current;
        new_node->right = next;

        *linked_list = (*linked_list)->next->next;

        insert_ordered_in_linked_list(linked_list, new_node);
    }

    huffman_tree->linkedList = *linked_list;
    huffman_tree->tree_size  = (tree_size * 2) - 1;
    huffman_tree->trash_size = 0;
    huffman_tree->dictionary = dictionary;
    huffman_tree->preorder   = (char *) malloc(huffman_tree->tree_size * sizeof(char));

    return huffman_tree;
}

int make_dictionary(linked_list_t **linked_list, unsigned long ***dictionary,
                    char **pre_order_tree, unsigned int binary_word)
{
    linked_list_t *current = *linked_list;

    static int dictionary_index = 0;
    static int pre_order_index  = 0;

    if (linked_list == NULL) {
        return ERR_NULL_POINTER;
    }

    char *byte = (char *) malloc(sizeof(char));
    byte       = &((byte_frequency_t *) ((*linked_list)->data))->byte;

    (*pre_order_tree)[pre_order_index] = *byte;
    pre_order_index += 1;

    if ((*linked_list)->left != NULL) {
        make_dictionary(&(*linked_list)->left, dictionary, pre_order_tree,
                        binary_word << 1 | 0);
    }

    if ((*linked_list)->right != NULL) {
        make_dictionary(&(*linked_list)->right, dictionary, pre_order_tree,
                        binary_word << 1 | 1);
    }

    if (((*linked_list)->right == NULL) && ((*linked_list)->left == NULL)) {
        unsigned long *dictionary_item =
            (unsigned long *) malloc(2 * sizeof(unsigned long));

        dictionary_item[0] = (unsigned long) (*linked_list)->data->byte;
        dictionary_item[1] = binary_word;

        (*dictionary)[dictionary_index] = dictionary_item;

        dictionary_index++;

#if DEBUG_MODE
        printf("%c , 0x%x, %d\n", (*linked_list)->data->byte, binary_word,
               dictionary_index);
#endif
    }

    return 0;
}

static char *make_header(huffman_tree_t *huffman_tree)
{
    /* Verificação de ponteiro. */
    if (huffman_tree == NULL) {
        return NULL;
    }

    /* Aloca memória para o cabeçalho. */
    char *header = (char *) malloc(((2 + huffman_tree->tree_size) * sizeof(char)));

    /* Garante que os bytes estarão zerados para as operações booleanas que se sucedem. */
    header[0] = 0;
    header[1] = 0;

    /* Procedimento de gravação dos dois primeiros bytes do cabeçalho. */
    header[0] = header[0] | (huffman_tree->trash_size << 5);
    header[0] = header[0] | (huffman_tree->tree_size >> 8);
    header[1] = header[1] | (huffman_tree->tree_size & 0xF);
    /* Procedimento de gravação dos dois primeiros bytes do cabeçalho:
     *
     * header = 0000 0000 0000 0000
     *
     * trash_size = 0000 0xxx
     * trash_size << 5 = xxx0 0000
     *
     *      header[0] = 0000 0000
     *      OR          xxx0 0000
     *      =           xxx0 0000
     *
     * tree_size = 000h hhhh hhhh hhhh
     * tree_size >> 8 = 0000 0000 000h hhhh
     *
     *      header[0] =           xxx0 0000
     *      OR          0000 0000 000h hhhh
     *      =                     xxxh hhhh
     *
     * tree_size & 0xF =
     *                  tree_size = 000h hhhh hhhh hhhh
     *                  0xF       = 0000 0000 1111 1111
     *                  =           0000 0000 hhhh hhhh
     *
     *      header[1] =           0000 0000
     *      OR          0000 0000 hhhh hhhh
     *      =                     hhhh hhhh
     *
     *      header = xxxh hhhh hhhh hhhh.
     */


    /* Inclui a árvore em pré ordem no cabeçalho. */
    for (int x = 0; x < huffman_tree->tree_size; x++) {
        header[2 + x] = huffman_tree->preorder[x];
    }

#if DEBUG_MODE
    printf("\nHeader: ");
    printf_bit_to_bit(header[0]);
    printf_bit_to_bit(header[1]);
    printf("\n");

    for (int x = 0; x < huffman_tree->tree_size; x++) {
        printf("%c", header[2 + x]);
    }
#endif

    return header;
}

static int insert_header(const char *input_file_name, const void *data, size_t size_data)
{
    /* Definição das variáveis. */
    FILE *input_file;
    char *buffer = NULL;
    long last_size;

    /* Verífica se os ponteiros são válidos. */
    if (input_file_name == NULL || data == NULL) {
        return ERR_NULL_POINTER;
    }

    /* Abre o arquvivo no modo de leitura e escrita binária */
    input_file = fopen(input_file_name, "r+b");

    /* Verifica se o arquivo é válido */
    if (input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    /**
     * Para descobrir o tamanho atual do arquivo precisamos posicionar o ponteiro no
     * final do arquivo, em seguida retornar a posição atual do ponteiro:
     */

    fseek(input_file, 0, SEEK_END); /* Posiciona o ponteiro no final do arquivo. */
    last_size = ftell(input_file);  /* Retorna a posição atual do ponteiro. */

    /* Aloca memória temporariamente para copiar os dados do arquivo. */
    buffer = (char *) malloc(last_size);

    /* Verifica se foi possível alocar memoria para os dados. */
    if (buffer == NULL) {
        fclose(input_file);
        return ERR_ALLOC_MEM;
    }

    /* Posiciona o ponteiro novamente no ínicio do arquivo. */
    rewind(input_file);

    /* Cópia os dados do arquivo. */
    fread(buffer, 1, last_size, input_file);

    /* Posicione o ponteiro novamente no início do arquivo. */
    rewind(input_file);

    /* Escreva os novos dados no início do arquivo */
    fwrite(data, 1, size_data, input_file);

    /* Escreva o conteúdo anterior de volta após os novos dados */
    fwrite(buffer, 1, last_size, input_file);

    free(buffer);
    fclose(input_file);

    return 0;
}

int compress_file(char *input_file_name, char *output_file_name,
                  huffman_tree_t *huffman_tree)
{
    char *header = (char *) malloc(((2 + huffman_tree->tree_size) * sizeof(char)));

    char output_byte            = 0;
    unsigned int cache_buffer   = 0;
    unsigned int shift_cache    = 0;
    unsigned int target_eq_byte = 0;
    unsigned int trash_size     = 0;

    /* Ponteiro para o arquivo no modo leitura binária "rb" */
    FILE *input_file  = fopen(input_file_name, "rb");
    FILE *output_file = fopen(output_file_name, "wb");

    if (input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    while (1) {
        /* Alocação de memória para armazenar provisóriamente o byte lido no arquivo */
        char *input_byte = malloc(sizeof(char));

        if (fread(input_byte, 1, 1, input_file) != 1) {
            /* Libera a memória reservada para byte antes de sair do loop */
            free(input_byte);
            break;
        }

        for (int scan_dict = 0; scan_dict <= huffman_tree->tree_size / 2; scan_dict++) {
            char target_byte = (char) huffman_tree->dictionary[scan_dict][0];
            target_eq_byte   = huffman_tree->dictionary[scan_dict][1];
            if (target_byte == *input_byte) {
                unsigned long depth =
                    find_depth_in_huffman_tree(huffman_tree->linkedList, &target_byte, 0);

                while (shift_cache + depth >= 8) {
                    unsigned int shift_for_output = (depth - (shift_cache + depth - 8));
                    unsigned int shift_for_cache  = ((shift_cache + depth) - 8);

                    output_byte = output_byte << shift_for_output;
                    output_byte = output_byte | (target_eq_byte >> shift_for_cache);

                    cache_buffer = cache_buffer << shift_for_cache;
                    cache_buffer = cache_buffer | (target_eq_byte >> shift_for_cache);
                    size_t write =
                        fwrite(&output_byte, sizeof(output_byte), 1, output_file);

                    if (write != 1) {
                        fclose(output_file);
                        return ERR_FILE_WRITE;
                    }

                    // printf_bit_to_bit(output_byte);

                    shift_cache = 0;
                    depth       = depth - shift_for_output;
                }

#if DEBUG_MODE
                printf("Pre-shift: ");
                printf_bit_to_bit(output_byte);
#endif
                output_byte = output_byte << depth;
                output_byte = output_byte | target_eq_byte;
                shift_cache += depth;

#if DEBUG_MODE
                printf("shift cache: %d, depth: %lu -> ", shift_cache, depth);
                printf_bit_to_bit(output_byte);
                printf("\n");
#endif
                break;
            }
        }
    }

    if (shift_cache) {
        trash_size  = 8 - shift_cache;
        output_byte = output_byte << trash_size;
        output_byte = output_byte | (0xFF >> shift_cache);

        huffman_tree->trash_size = trash_size;
    }

    header = make_header(huffman_tree);
    fclose(input_file);
    fclose(output_file);

    insert_header(output_file_name, header, strlen(header));

    return 0;
}