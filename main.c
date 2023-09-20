#include <common.h>
#include <input_process.h>
#include <interface.h>
#include <output_process.h>
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

    file_header_t *header = (file_header_t *) malloc(sizeof(file_header_t));

    get_header_from_file("output1.gip", &header);

    int index = 0;

    huffman_tree_t *huffman_output = (huffman_tree_t *) malloc(sizeof(huffman_tree_t));

    huffman_output->linkedList = create_tree_from_preorder(header->preorder_tree, &index);

    //TODO: Implementar função para executar isso ai.
    huffman_output->preorder = (char *) malloc(huffman_output->tree_size * sizeof(char));
    huffman_output->tree_size = header->tree_size;
    huffman_output->trash_size = header->trash_size;
    huffman_output->preorder = header->preorder_tree;

    huffman_output->dictionary = (unsigned long **) malloc(
        ((huffman_output->tree_size / 2)) * sizeof(unsigned long *));

    make_preorder_dictionary(STATIC_RESET);

    make_preorder_dictionary(&huffman_output->linkedList, &huffman_output->dictionary,
                             &huffman_output->preorder, 0);

    extract_file("output1.gip", "output.txt", huffman_output);

    return 0;
}
