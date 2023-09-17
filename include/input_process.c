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


int count_nodes(linked_list_t *head)
{
    int count              = 0;
    linked_list_t *current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count - 1;
}

ruffman_tree_t *make_ruffman_tree(linked_list_t **linked_list)
{
    ruffman_tree_t *ruffman_tree = (ruffman_tree_t *) malloc(sizeof(ruffman_tree_t));

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

    ruffman_tree->linkedList = *linked_list;
    ruffman_tree->tree_size  = (tree_size * 2) - 1;
    ruffman_tree->trash_size = 0;
    ruffman_tree->dictionary = dictionary;

    return ruffman_tree;
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
        // TODO: implementar geração do dicionário.
        unsigned long *dictionary_item =
            (unsigned long *) malloc(2 * sizeof(unsigned long));

        dictionary_item[0] = (unsigned long) (*linked_list)->data->byte;
        dictionary_item[1] = binary_word;

        (*dictionary)[dictionary_index] = dictionary_item;

        dictionary_index++;

        printf("%c , 0x%x, %d\n", (*linked_list)->data->byte, binary_word,
               dictionary_index);
    }

    return 0;
}

int compress_file(char *input_file_name, char *output_file_name, ruffman_tree_t *ruffman_tree)
{
    /* Ponteiro para o arquivo no modo leitura binária "rb" */
    FILE *input_file = fopen(input_file_name, "rb");

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
    }

    fclose(input_file);
    return 0;
}