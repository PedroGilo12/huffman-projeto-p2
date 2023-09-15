/**
 * @file common.c
 *
 * @authors
 *
 * @brief
 */

#include <common.h>

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

linked_list_t *exist_in_linked_list(linked_list_t *linked_list, const char *target_char)
{
    linked_list_t *current = linked_list;

    while (current != NULL) {
        char *dataChar = (char *) current->data;
        if (dataChar != NULL && *dataChar == *target_char) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int insert_at_beginning(linked_list_t **head, void *data)
{
    linked_list_t *new_node = (linked_list_t *) malloc(sizeof(linked_list_t));
    if (new_node == NULL) {
        return -1;
    }

    byte_frequency_t *byte_frequency =
        (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

    byte_frequency->byte      = ((char *) data)[0];
    byte_frequency->frequency = 1;

    new_node->data = (byte_frequency_t *) byte_frequency;
    new_node->next = *head;
    *head          = new_node;

    return 0;
}

void swap_nodes(linked_list_t **head_ref, linked_list_t *node1, linked_list_t *node2)
{
    if (node1 == node2) {
        return;
    }

    linked_list_t *previous = NULL, *prev2 = NULL, *current = *head_ref;

    while (current != NULL && current != node1) {
        previous = current;
        current = current->next;
    }

    current = *head_ref;
    while (current != NULL && current != node2) {
        prev2   = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

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
void sort_linked_list_by_frequency(linked_list_t **head_ref)
{
    int swapped;

    if (*head_ref == NULL || (*head_ref)->next == NULL) {
        return;
    }

    do {
        swapped                = 0;
        linked_list_t *current = *head_ref;

        while (current->next != NULL) {
            byte_frequency_t *data1 = (byte_frequency_t *) (current->data);
            byte_frequency_t *data2 = (byte_frequency_t *) (current->next->data);

            if (data1 != NULL && data2 != NULL && data1->frequency > data2->frequency) {
                swap_nodes(head_ref, current, current->next);
                swapped = 1;
            }

            current = current->next;
        }
    } while (swapped);
}

void print_linked_list_byte_frequency(linked_list_t *linked_list)
{
    linked_list_t *current = linked_list;

    while (current->data != NULL) {
        printf("Byte: 0x%x, Frequency: %lu\n",
               ((byte_frequency_t *) current->data)->byte,
               ((byte_frequency_t *) current->data)->frequency);

        current = current->next;
    }
}
