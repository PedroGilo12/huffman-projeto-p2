/**
 * @file input_process.c
 *
 * @brief Responsável por receber e tratar o arquivo de entrada.
 *
 * @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
 *          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
 *          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
 *          Raniel Ferreira Athayde (rfa@ic.ufal.br)
 */

#include "interface.h"

#define N_ATTEMPTS_TO_OPEN_FILE 9

int get_file_names(char* input_file_name, char* output_file_name);

void main_interface(void)
{
    char* input_file_name  = (char*) malloc(64 * sizeof(char));
    char* output_file_name = (char*) malloc(64 * sizeof(char));

    get_file_names(input_file_name, output_file_name);

    printf("Arquivo de entrada: %s\nArquivo de saida: %s\n", input_file_name,
           output_file_name);

    //    linked_list_t *linked_list = create_empty_linked_list();
    //
    //    process_input_file_as_byte_frequency("input.txt", &linked_list);
    //
    //    sort_linked_list_by_frequency(&linked_list);
    //
    //    print_linked_list_byte_frequency(linked_list);
    //
    //    huffman_tree_t *huffman_tree = make_huffman_tree(&linked_list);
    //
    //    make_preorder_dictionary(&huffman_tree->linkedList, &huffman_tree->dictionary,
    //                             &huffman_tree->preorder, 0);
    //
    //    compress_file("input.txt", "output1.gip", huffman_tree);
}

int get_file_names(char* input_file_name, char* output_file_name)
{
    char* input = (char*) malloc(64 * sizeof(char));

    printf("Digite o caminho do arquivo que deseja compactar: \n");
    scanf("%s", input_file_name);
    strcpy(input, input_file_name);

    for (int index = 0; index < strlen(input_file_name); index++) {
        if (input_file_name[index] == '.') {
            input_file_name[index] = '\0';
        }
    }

    char* input_cache = (char*) malloc(64 * sizeof(char));
    strcpy(input_cache, input_file_name);

    strcat(input_file_name, ".gip");

    for (int attempts = 1; attempts < N_ATTEMPTS_TO_OPEN_FILE; attempts++) {
        if (fopen(input_file_name, "r") == NULL) {
            break;
        }

        char* suffix = (char*) malloc(4 * sizeof(char));
        sprintf(suffix, "(%d)", attempts);
        strcpy(input_file_name, input_cache);
        strcat(input_file_name, suffix);
        strcat(input_file_name, ".gip");

        printf("Não foi possivel abrir o arquivo\n");
    }

    FILE* output_file = fopen(input_file_name, "w");

    strcpy(output_file_name, input_file_name);
    strcpy(input_file_name, input);
    fclose(output_file);
}