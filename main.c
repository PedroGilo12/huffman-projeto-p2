#include <common.h>
#include <input_process.h>
#include <stdio.h>

int main()
{
    linked_list_t *linked_list = create_empty_linked_list();

    process_input_file_as_byte_frequency("input.txt", &linked_list);

    sort_linked_list_by_frequency(&linked_list);

    print_linked_list_byte_frequency(linked_list);

    huffman_tree_t *huffman_tree = make_huffman_tree(&linked_list);

    make_preorder_dictionary(&huffman_tree->linkedList, &huffman_tree->dictionary,
                             &huffman_tree->preorder, 0);

    compress_file("input.txt", "output1.gip", huffman_tree);

    return 0;
}
