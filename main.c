#include <stdio.h>
#include <common.h>
#include <input_process.h>

int main() {
    linked_list_t *linked_list = create_empty_linked_list();

    process_input_file_as_byte_frequency("input.txt", &linked_list);

    sort_linked_list_by_frequency(&linked_list);

    print_linked_list_byte_frequency(linked_list);

    return 0;
}
