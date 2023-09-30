/**
 * @file common.c
 *
 * @brief Funções utilizadas em todos os arquivos do programa.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde (rfa@ic.ufal.br)
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
                                         const unsigned char *target_char)
{
    /* Clona o ponteiro para o nó atual */
    linked_list_t *current = linked_list;

    while (current != NULL) {
        /* Clona o ponteiro para o dado do nó atual*/
        unsigned char *data_char = (unsigned char *) current->data;
        if (data_char != NULL && *data_char == *target_char) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int insert_byte_frequency_at_beginning(linked_list_t **head, const unsigned char *byte,
                                       unsigned long frequency)
{
    /* Aloca memória para um novo nó */
    linked_list_t *new_node = create_empty_linked_list();

    /* Verífica se os ponteiros são válidos */
    if ((new_node == NULL) || (*head == NULL) || (byte == NULL)) {
        return ERR_NULL_POINTER;
    }

    /* Aloca memória para novo dado*/
    byte_frequency_t *byte_frequency =
        (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

    /* Configura o dado */
    byte_frequency->byte      = ((unsigned char *) byte)[0];
    byte_frequency->frequency = frequency;

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

    linked_list_t *previous  = NULL;
    linked_list_t *previous2 = NULL;
    linked_list_t *current   = *head_ref;

    /* Encontra o nó1 na lista */
    while (current != NULL && current != node1) {
        previous = current;
        current  = current->next;
    }

    current = *head_ref;

    /* Encontra o nó2 na lista */
    while (current != NULL && current != node2) {
        previous2 = current;
        current   = current->next;
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

    if (previous2 != NULL) {
        previous2->next = node1;
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

bool compare_byte_frequency(byte_frequency_t *data1, byte_frequency_t *data2)
{
    return data1->frequency > data2->frequency;
}

int insert_ordered_in_linked_list(linked_list_t **linked_list, linked_list_t *new_node)
{
    if (linked_list == NULL || new_node == NULL) {
        return ERR_NULL_POINTER;
    }

    /* Caso especial: lista vazia ou inserção no início */
    if ((*linked_list)->data == NULL
        || !compare_byte_frequency(new_node->data, (*linked_list)->data)) {
        new_node->next = *linked_list;
        *linked_list   = new_node;
        return 0;
    }

    linked_list_t *previous = *linked_list;
    linked_list_t *current  = (*linked_list)->next;

    while (current->data != NULL
           && compare_byte_frequency(new_node->data, current->data)) {
        previous = current;
        current  = current->next;
    }

    new_node->next = current;
    previous->next = new_node;

    return 0;
}

void print_linked_list_byte_frequency(linked_list_t *linked_list)
{
    linked_list_t *current = linked_list;

    while (current->data != NULL) {
        printf("Byte: 0x%x, char: %c, Frequency: %lu\n",
               ((byte_frequency_t *) current->data)->byte,
               ((byte_frequency_t *) current->data)->byte,
               ((byte_frequency_t *) current->data)->frequency);

        current = current->next;
    }
    printf("Finished!\n");
}

linked_list_t *new_node(void *data)
{
    linked_list_t *node = (linked_list_t *) malloc(sizeof(linked_list_t));
    node->data          = (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

    node->data->byte = *(unsigned char *) data;  // Explique de novo!
    ((byte_frequency_t *) (node->data))->frequency = 0;
    node->left                                     = NULL;
    node->right                                    = NULL;

    return node;
}

bool compare_data_in_byte_frequency(byte_frequency_t *byte_frequency, void *received_data)
{
    unsigned char data1 = byte_frequency->byte;
    unsigned char data2 = ((byte_frequency_t *) received_data)->byte;
    return data1 == data2;
}

/* Função para encontrar a profundidade de um nó na árvore */
unsigned long find_depth_in_huffman_tree(linked_list_t *linked_list, void *target,
                                         unsigned long depth)
{
    /* Caso base: se a árvore estiver vazia, retorne -1 */
    if (linked_list == NULL || linked_list->data == NULL) {
        return 0;
    }

    /* Se o nó atual tiver o valor desejado, retorne a profundidade */
    if ((compare_data_in_byte_frequency(linked_list->data, (byte_frequency_t *) target)) && (linked_list->left == NULL) && (linked_list->right == NULL)) {
        return depth;
    }

    /* Caso contrário, procure nas subárvores esquerda e direita */
    unsigned long left_depth =
        find_depth_in_huffman_tree(linked_list->left, target, depth + 1);
    unsigned long right_depth =
        find_depth_in_huffman_tree(linked_list->right, target, depth + 1);

    /* Se o nó não for encontrado em nenhuma subárvore, retorne -1 */
    if (left_depth == 0 && right_depth == 0) {
        return 0;
    }

    /* Retorna a profundidade máxima encontrada nas subárvores */
    return (left_depth > right_depth) ? left_depth : right_depth;
}

int count_nodes(linked_list_t *head)
{
    int count              = 0;
    linked_list_t *current = head;

    while (current->data != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

void printf_bit_by_bit(unsigned char byte)
{
    for (int i = 7; i >= 0; i--) {
        unsigned char mask = 1 << i;
        unsigned char bit  = (byte & mask) ? '1' : '0';
        printf("%c", bit);
    }
    printf(" ");
}
