/**
 * @file common.c
 *
 * @authors
 *
 * @brief
*/

#include <common.h>

linked_list_t *create_empty_linked_list() {

    /* Aloca memória para uma nova lista encadeada */
    linked_list_t *linked_list = (linked_list_t *)malloc(sizeof(linked_list_t));

    /* Atribui os valores da lista vazia */
    linked_list->data = NULL;
    linked_list->next = NULL;
    linked_list->left = NULL;
    linked_list->right = NULL;

    return linked_list;
}

linked_list_t * exist_in_linked_list(linked_list_t *linked_list, const char * target_char) {
    linked_list_t *current = linked_list;

    while (current != NULL) {

        char *dataChar = (char *)current->data;
        if (dataChar != NULL && *dataChar == *target_char) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int insert_at_beginning(linked_list_t **head, void *data) {

    linked_list_t *new_node = (linked_list_t *)malloc(sizeof(linked_list_t));
    if (new_node == NULL) {
        return -1;
    }

    byte_frequency_t * byte_frequency = (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

    byte_frequency->byte = ((char*)data)[0];
    byte_frequency->frequency = 1;

    new_node->data = (byte_frequency_t *) byte_frequency;
    new_node->next = *head;
    *head = new_node;

    return 0;
}