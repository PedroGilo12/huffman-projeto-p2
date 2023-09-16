/**
 * @file input_process.c
 *
 * @brief Responsável por receber e tratar o arquivo de entrada.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde ()
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
        return -1;
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

int make_ruffman_tree(linked_list_t **linked_list) {
    if (*linked_list == NULL || (*linked_list)->next == NULL) {
        return ERR_NULL_POINTER;
    }

    sort_linked_list_by_frequency(linked_list);

    while ((*linked_list)->next->data != NULL) {
        linked_list_t *current = *linked_list;
        linked_list_t *next = current->next;

        byte_frequency_t *new_byte_frequency =
            (byte_frequency_t *)malloc(sizeof(byte_frequency_t));

        if (new_byte_frequency == NULL) {
            return ERR_NULL_POINTER;
        }

        unsigned long current_byte_frequency = ((byte_frequency_t *)current->data)->frequency;
        unsigned long next_byte_frequency = ((byte_frequency_t *)next->data)->frequency;

        new_byte_frequency->byte = '*';
        new_byte_frequency->frequency = current_byte_frequency + next_byte_frequency;

        linked_list_t *new_node = (linked_list_t *)malloc(sizeof(linked_list_t));

        if (new_node == NULL) {
            return ERR_NULL_POINTER;
        }

        new_node->data = (byte_frequency_t *)new_byte_frequency;
        new_node->left = current;
        new_node->right = next;

        *linked_list = (*linked_list)->next->next;

        insert_ordered_in_linked_list(linked_list, new_node);

    }

    return 0;
}
