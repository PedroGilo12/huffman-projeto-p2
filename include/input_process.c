//
// Created by Pedro on 14/09/2023.
//

#include <input_process.h>

int process_input_file(const char *file_name, linked_list_t **linked_list) {
    char *byte;

    if ((file_name == NULL) || (linked_list == NULL)) {
        return -1;
    }

    FILE *input_file = fopen(file_name, "rb");

    if (input_file == NULL) {
        return -1;
    }

    while (1) {
        byte = malloc(sizeof(char));

        if (fread(byte, 1, 1, input_file) != 1) {
            free(byte); // Libere a memória alocada para 'byte' quando você sair do loop
            break;
        }

        linked_list_t *current = exist_in_linked_list(*linked_list, byte);

        if (current != NULL) {
            ((byte_frequency_t *)current->data)->frequency += 1;
            printf("Caracter já existe %c, frequência: %lu\n", ((byte_frequency_t *)current->data)->byte,
                   ((byte_frequency_t *)current->data)->frequency);
        } else {
            insert_at_beginning(linked_list, byte);
            printf("Caracter inserido %c\n", byte[0]);
        }
    }

    fclose(input_file);
    return 0;
}
