/**
 * @file common.c
 *
 * @brief Funções utilizadas em todos os arquivos do programa.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde ()
 */

#include <common.h>

/**
 * @brief Troca dois nós em uma lista encadeada.
 *
 * @param head_ref Ponteiro duplo para o cabeça da lista encadeada.
 * @param node1 Ponteiro para o primeiro nó a ser trocado.
 * @param node2 Ponteiro para o segundo nó a ser trocado.
 */
static void swap_nodes(linked_list_t **head_ref, linked_list_t *node1,
                       linked_list_t *node2);

linked_list_t *create_empty_linked_list()
{
    /* Aloca memória para uma nova lista encadeada */
    linked_list_t *linked_list = (linked_list_t *) malloc(sizeof(linked_list_t));

    /* Atribui os valores da lista vazia */
    linked_list->data  = NULL;
    linked_list->next  = NULL;
    linked_list->left  = NULL;
    linked_list->right = NULL;

    return linked_list;
}

linked_list_t *exist_byte_in_linked_list(linked_list_t *linked_list,
                                         const char *target_char)
{
    /* Clona o ponteiro para o nó atual */
    linked_list_t *current = linked_list;

    while (current != NULL) {
        /* Clona o ponteiro para o dado do nó atual*/
        char *data_char = (char *) current->data;
        if (data_char != NULL && *data_char == *target_char) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int insert_byte_frequency_at_beginning(linked_list_t **head, void *data)
{
    /* Aloca memória para um novo nó */
    linked_list_t *new_node = (linked_list_t *) malloc(sizeof(linked_list_t));

    /* Verífica se os ponteiros são válidos */
    if ((new_node == NULL) || (*head == NULL) || (data == NULL)) {
        return ERR_NULL_POINTER;
    }

    /* Aloca memória para novo dado*/
    byte_frequency_t *byte_frequency =
        (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

    /* Configura o dado */
    byte_frequency->byte      = ((char *) data)[0];
    byte_frequency->frequency = 1;

    /* Configura o nó */
    new_node->data = (byte_frequency_t *) byte_frequency;
    new_node->next = *head;

    /* Troca o cabeça da lista encadeada */
    *head = new_node;

    return 0;
}

static void swap_nodes(linked_list_t **head_ref, linked_list_t *node1,
                       linked_list_t *node2)
{
    /* Verifica se os nós são os mesmos */
    if (node1 == node2) {
        return;
    }

    linked_list_t *previous = NULL, *prev2 = NULL, *current = *head_ref;

    /* Encontra o nó1 na lista */
    while (current != NULL && current != node1) {
        previous = current;
        current  = current->next;
    }

    current = *head_ref;

    /* Encontra o nó2 na lista */
    while (current != NULL && current != node2) {
        prev2   = current;
        current = current->next;
    }

    /* Se algum dos nós não for encontrado, não faça a troca */
    if (current == NULL) {
        return;
    }

    /* Atualiza os ponteiros para realizar a troca */
    if (previous != NULL) {
        previous->next = node2;
    } else {
        *head_ref = node2;
    }

    if (prev2 != NULL) {
        prev2->next = node1;
    } else {
        *head_ref = node1;
    }

    linked_list_t *temp = node1->next;
    node1->next         = node2->next;
    node2->next         = temp;
}

int sort_linked_list_by_frequency(linked_list_t **linked_list)
{
    /**
     * Este algorítmo representa um bubblesort para uma lista encadeada.
     */

    /* flag para indicar se ainda há trocas para serem feitas */
    int swapped;

    /* Verífica se os ponteiros são válidos */
    if (*linked_list == NULL || (*linked_list)->next == NULL) {
        return ERR_NULL_POINTER;
    }

    do {
        swapped = 0;

        /* Cópia o ponteiro para o nó atual */
        linked_list_t *current = *linked_list;

        /* Enquanto não chegar no final da lista */
        while (current->next != NULL) {
            /* Clona o ponteiro para o dado do nó atual e do próximo nó */
            byte_frequency_t *data1 = (byte_frequency_t *) (current->data);
            byte_frequency_t *data2 = (byte_frequency_t *) (current->next->data);

            /* Se os ponteiros forem válidos e a frequência do nó atual for maior que a do
             * próximo nó */
            if (data1 != NULL && data2 != NULL && data1->frequency > data2->frequency) {
                swap_nodes(linked_list, current, current->next);
                swapped = 1;
            }

            current = current->next;
        }
    } while (swapped);

    return 0;
}

void print_linked_list_byte_frequency(linked_list_t *linked_list)
{
    linked_list_t *current = linked_list;

    while (current->data != NULL) {
        printf("Byte: 0x%x, Frequency: %lu\n", ((byte_frequency_t *) current->data)->byte,
               ((byte_frequency_t *) current->data)->frequency);

        current = current->next;
    }
}
