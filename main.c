#include <common.h>
#include <input_process.h>
#include <stdio.h>

int main()
{
    linked_list_t *linked_list = create_empty_linked_list();

    process_input_file_as_byte_frequency("input.jpeg", &linked_list);

    sort_linked_list_by_frequency(&linked_list);

    print_linked_list_byte_frequency(linked_list);

    ruffman_tree_t *ruffmanTree = make_ruffman_tree(&linked_list);

    make_dictionary(&ruffmanTree->linkedList, &ruffmanTree->dictionary,
                    &ruffmanTree->preorder, 0);

    compress_file("input.jpeg", "output.gip", ruffmanTree);

    return 0;
}
