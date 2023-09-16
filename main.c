#include <stdio.h>
#include <common.h>
#include <input_process.h>

int main() {
    linked_list_t *linked_list = create_empty_linked_list();

    process_input_file_as_byte_frequency("input.txt", &linked_list);

    sort_linked_list_by_frequency(&linked_list);

    print_linked_list_byte_frequency(linked_list);

    ruffman_tree_t *ruffmanTree = make_ruffman_tree(&linked_list);

    char * preorder = (char *) malloc(ruffmanTree->tree_size * sizeof(char));

    int index = 0;

    make_dictionary(&ruffmanTree->linkedList,&preorder, &index);

    for(int x = 0; x < index; x++) {
        printf("%c",preorder[x]);
    }

    return 0;
}
