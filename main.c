#include <common.h>
#include <input_process.h>
#include <interface.h>
#include <output_process.h>
#include <stdio.h>

#define COMPRESS 0

const unsigned char input_file_name[] = "input.mp4";
const unsigned char output_compress_file_name[] = "output.gip";
const unsigned char output_file_name[] = "output.mp4";

int main()
{
    //************************ COMPRESSÃO **********************************************
#if COMPRESS


    linked_list_t *linked_list = create_empty_linked_list();

    process_input_file_as_byte_frequency(input_file_name, &linked_list);

    printf("Entrada Processada|\n");
    sort_linked_list_by_frequency(&linked_list);

    print_linked_list_byte_frequency(linked_list);

    huffman_tree_t *huffman_tree = make_huffman_tree(&linked_list);

    printf("Arvore criada, tamanho da arvore: %lu\n", huffman_tree->tree_size);

    make_preorder_dictionary(&huffman_tree->linkedList, &huffman_tree->dictionary,
                             &huffman_tree->preorder, 0);

    printf("Dicionario Criado|\n");

    compress_file(input_file_name, output_compress_file_name, huffman_tree);

#else

    //***********************************************************************************

    //************************** DESCOMPRESSÃO ******************************************


    file_header_t *header = (file_header_t *) malloc(sizeof(file_header_t));

    get_header_from_file(output_compress_file_name, &header);

    int index = 0;

    huffman_tree_t *huffman_output = (huffman_tree_t *) malloc(sizeof(huffman_tree_t));

    huffman_output->linkedList = create_tree_from_preorder(header->preorder_tree, &index);

    //TODO: Implementar função para executar isso ai.
    huffman_output->preorder = (unsigned char *) malloc(huffman_output->tree_size * sizeof(char));
    huffman_output->tree_size = header->tree_size;
    huffman_output->trash_size = header->trash_size;
    huffman_output->preorder = header->preorder_tree;

    huffman_output->dictionary = (unsigned long **) malloc(
        ((huffman_output->tree_size / 2)) * sizeof(unsigned long *));

    make_preorder_dictionary(STATIC_RESET);

    make_preorder_dictionary(&huffman_output->linkedList, &huffman_output->dictionary,
                             &huffman_output->preorder, 0);

    extract_file(output_compress_file_name, output_file_name, huffman_output);

#endif
    //***********************************************************************************
    return 0;
}
