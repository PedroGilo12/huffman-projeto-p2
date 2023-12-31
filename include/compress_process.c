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

#include "compress_process.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Insere o cabeçalho no início do arquivo.
 *
 * @param input_file_name Nome do arquivo.
 * @param data Cabeçalho.
 * @param size_data Tamanho do cabeçalho.
 *
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
static int insert_header(const char *input_file_name, unsigned char *data,
                         size_t size_data);

int process_input_file_as_byte_frequency(const char *file_name,
                                         linked_list_t **linked_list)
{
    /* Verificação de ponteiro para garantir que os ponteiros são válidos */
    if ((file_name == NULL) || (linked_list == NULL)) {
        return ERR_NULL_POINTER;
    }

    /* Ponteiro para o arquivo no modo leitura binária "rb" */
    FILE *input_file = fopen((char *) file_name, "rb");

    if (input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    while (1) {
        /* Alocação de memória para armazenar provisoriamente o byte lido no arquivo */
        unsigned char *byte = (unsigned char *) malloc(sizeof(unsigned char));

        if (fread(byte, 1, 1, input_file) != 1) {
            /* Libera a memória reservada para byte antes de sair do loop */
            free(byte);
            break;
        }

        /* Nó atual da lista encadeada */
        linked_list_t *current = exist_byte_in_linked_list(*linked_list, byte);

        if (current != NULL) {
            /* Se o nó atual já existir, apenas adicionar mais um à frequência */
            ((byte_frequency_t *) current->data)->frequency += 1;

#if DEBUG_MODE
            printf("Caracter já existe %c, HEX: 0x%x frequência: %lu\n",
                   ((byte_frequency_t *) current->data)->byte,
                   ((byte_frequency_t *) current->data)->byte,
                   ((byte_frequency_t *) current->data)->frequency);
#endif

        } else {
            /* Se o nó não existir, inserir o nó no começo da lista */
            insert_byte_frequency_at_beginning(linked_list, byte, 1);

#if DEBUG_MODE
            printf("Caracter inserido %c, HEX: 0x%x\n", byte[0], byte[0]);
#endif
        }
    }

    fclose(input_file);
    return 0;
}

huffman_tree_t *make_huffman_tree(linked_list_t **linked_list)
{
    /* Aloca memória para a árvore de Huffman. */
    huffman_tree_t *huffman_tree = (huffman_tree_t *) malloc(sizeof(huffman_tree_t));

    unsigned long tree_size = count_nodes(*linked_list);
    unsigned long **dictionary =
        (unsigned long **) malloc(tree_size * sizeof(unsigned long *));

    /* Verificação de ponteiro. */
    if (*linked_list == NULL || (*linked_list)->next == NULL) {
        return NULL;
    }

    /* Garante que a lista inicie ordenada. */
    sort_linked_list_by_frequency(linked_list);

    while ((*linked_list)->next->data != NULL) {
        /* Ponteiros para percorrer a lista. */
        linked_list_t *current = *linked_list;
        linked_list_t *next    = current->next;

        /* Aloca memória para o novo dado. */
        byte_frequency_t *new_byte_frequency =
            (byte_frequency_t *) malloc(sizeof(byte_frequency_t));

        if (new_byte_frequency == NULL) {
            return NULL;
        }

        /* Configuração do novo nó: byte: '*' frequency: +++ */
        unsigned long current_byte_frequency =
            ((byte_frequency_t *) current->data)->frequency;
        unsigned long next_byte_frequency = ((byte_frequency_t *) next->data)->frequency;

        new_byte_frequency->byte      = '*';
        new_byte_frequency->frequency = current_byte_frequency + next_byte_frequency;

        /* Aloca memória para o novo nó. */
        linked_list_t *new_node = (linked_list_t *) malloc(sizeof(linked_list_t));

        if (new_node == NULL) {
            return NULL;
        }

        /* Configura o novo nó */
        new_node->data  = (byte_frequency_t *) new_byte_frequency;
        new_node->left  = current;
        new_node->right = next;

        /* Desconecta os dois primeiros nós atuais da lista. */
        *linked_list = (*linked_list)->next->next;

        /* Adiciona ordenado o novo nó. */
        insert_ordered_in_linked_list(linked_list, new_node);
    }

    /* Atributos da Huffman Tree. */
    huffman_tree->linkedList = *linked_list;
    huffman_tree->tree_size  = (tree_size * 2) - 1;
    huffman_tree->trash_size = 0;
    huffman_tree->dictionary = dictionary;
    huffman_tree->preorder =
        (unsigned char *) malloc((2 + huffman_tree->tree_size) * sizeof(unsigned char));

    return huffman_tree;
}

int make_preorder_dictionary(linked_list_t **linked_list, unsigned long ***dictionary,
                             unsigned char **pre_order_tree, int binary_path)
{
    /* Variáveis estáticas para contabilizar o índice do dicionário e da string em pré
     * ordem. */
    static int dictionary_index = 0;
    static int pre_order_index  = 0;

    if (binary_path == -1) {
        dictionary_index = 0;
        pre_order_index  = 0;
        return 0;
    }

    /* Verifica se os ponteiros são válidos. */
    if (linked_list == NULL || dictionary == NULL) {
        return ERR_NULL_POINTER;
    }

    /* Cópia do ponteiro para o byte atual da árvore. */
    char *byte = &((byte_frequency_t *) ((*linked_list)->data))->byte;

    /* Adiciona o nó à string em pré ordem e soma +1 no indíce. */
    (*pre_order_tree)[pre_order_index] = *byte;
    pre_order_index += 1;

    /* Preferência para os filhos a esquerda, se não forem NULL. Prosseguir. */
    if ((*linked_list)->left != NULL) {
        /* Note que "binary_word << 1 | 0":
         * adiciona 0 ao caminho até a folha.*/
        make_preorder_dictionary(&(*linked_list)->left, dictionary, pre_order_tree,
                                 binary_path << 1 | 0);
    }

    /* Se não for possível ir para esquerda, vá para a direita. */
    if ((*linked_list)->right != NULL) {
        /* Note que "binary_word << 1 | 1":
         * adiciona 1 ao caminho até a folha.*/
        make_preorder_dictionary(&(*linked_list)->right, dictionary, pre_order_tree,
                                 binary_path << 1 | 1);
    }

    /* Se ambos os filhos, da esquerda e da direita foram NULL, então chegou em uma folha,
     * deve adicioanr essa folha ao dicionario. */
    if (((*linked_list)->right == NULL) && ((*linked_list)->left == NULL)) {
        /* Aloca memória para o item do dicionário. */
        unsigned long *dictionary_item =
            (unsigned long *) malloc(2 * sizeof(unsigned long));

        /* Atribui valores do item. */
        dictionary_item[0] =
            (unsigned long) ((byte_frequency_t *) (*linked_list)->data)->byte;
        dictionary_item[1] = binary_path;

        /* Inclui o item no dicionário. */
        (*dictionary)[dictionary_index] = dictionary_item;
        dictionary_index++;

        /* Casos especiais: Adição do caracter de escape a string em pré ordem. */
        if (((byte_frequency_t *) ((*linked_list)->data))->byte == '*'
            || ((byte_frequency_t *) ((*linked_list)->data))->byte == '\\') {
            /* Adição do caractere de escape. */
            (*pre_order_tree)[pre_order_index - 1] = '\\';
            (*pre_order_tree)[pre_order_index] =
                ((byte_frequency_t *) ((*linked_list)->data))->byte;

            pre_order_index++;
        }
#if DEBUG_MODE
        printf("%c , 0x%x, %d\n", (*linked_list)->data->byte, binary_word,
               dictionary_index);
#endif
    }

    return 0;
}

static unsigned char *make_header(huffman_tree_t *huffman_tree, unsigned int *size_head)
{
    /* Verificação de ponteiro. */
    if (huffman_tree == NULL) {
        return NULL;
    }

    /* Aloca memória para o cabeçalho. */
    unsigned char *header =
        (unsigned char *) malloc((2 + huffman_tree->tree_size) * sizeof(unsigned char));

    /* Garante que os bytes estarão zerados para as operações booleanas que se sucedem. */
    header[0] = 0;
    header[1] = 0;

    /* Insere a árvore em pré ordem no cabeçalho. */
    unsigned long insert_size = huffman_tree->tree_size;
    for (int x = 0; x < insert_size; x++) {
        /* Verifica se possui um caractere de escape e não contabiliza-o para gravação. */
        if (huffman_tree->preorder[x] == '\\' && huffman_tree->preorder[x + 1] != '\\') {
            insert_size += 1;
        }

        header[2 + x] = huffman_tree->preorder[x];
    }

    huffman_tree->tree_size = insert_size;

    /* Procedimento de gravação dos dois primeiros bytes do cabeçalho. */
    header[0] = header[0] | (huffman_tree->trash_size << 5);
    header[0] = header[0] | (huffman_tree->tree_size >> 8);
    header[1] = header[1] | (huffman_tree->tree_size & 0xFF);


#if DEBUG_MODE
    printf("\nHeader: ");
    printf_bit_by_bit(header[0]);
    printf_bit_by_bit(header[1]);
    printf("\n");

    for (int x = 0; x < insert_size; x++) {
        if (header[2 + x] < 10) {
            printf("0");
        }

        printf("%x ", header[2 + x]);
    }
#endif

    *size_head = insert_size + 2;

    return header;
}

static int insert_header(const char *input_file_name, unsigned char *data,
                         size_t size_data)
{
    /* Definição das variáveis. */
    FILE *input_file;
    unsigned char *buffer = NULL;
    long last_size;

    /* Verificação se os ponteiros são válidos. */
    if (input_file_name == NULL || data == NULL) {
        return ERR_NULL_POINTER;
    }

    /* Abre o arquivo no modo de leitura e escrita binária */
    input_file = fopen(input_file_name, "r+b");

    /* Verifica se o arquivo é válido */
    if (input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    /* Para descobrir o tamanho atual do arquivo, posicionamos o ponteiro no
     * final do arquivo e retornamos a posição atual do ponteiro. */
    fseek(input_file, 0, SEEK_END);
    last_size = ftell(input_file);

    /* Aloca memória temporariamente para copiar os dados do arquivo. */
    buffer = (unsigned char *) malloc(last_size);

    /* Verifica se foi possível alocar memória para os dados. */
    if (buffer == NULL) {
        fclose(input_file);
        return ERR_ALLOC_MEM;
    }

    /* Posiciona o ponteiro novamente no início do arquivo. */
    rewind(input_file);

    /* Copia os dados do arquivo. */
    fread(buffer, 1, last_size, input_file);

    /* Posicione o ponteiro novamente no início do arquivo. */
    fseek(input_file, 0, SEEK_SET);

    /* Escreva os novos dados no início do arquivo. */
    fwrite(data, 1, size_data, input_file);

    /* Escreva o conteúdo anterior de volta após os novos dados. */
    fwrite(buffer, 1, last_size, input_file);

    free(buffer);
    fclose(input_file);

    return 0;
}

int compress_file(const char *input_file_name, const char *output_file_name,
                  huffman_tree_t *huffman_tree)
{
    unsigned char *header =
        (unsigned char *) malloc((2 + huffman_tree->tree_size) * sizeof(unsigned char));

    /* Byte que será gravado no arquivo. */
    unsigned char output_byte = 0;

    /* Bits que estão aguardando envio. */
    unsigned int cache_buffer = 0;

    /* Quantidade de bits que foram alocados no byte de saída. */
    unsigned int shift_output_cache = 0;

    /* Byte equivalente do dicionário. */
    unsigned int target_eq_byte = 0;

    /* Tamanho do lixo. */
    unsigned int trash_size = 0;

    /* Ponteiros para os arquivos de entrada e saída. */
    FILE *input_file  = fopen(input_file_name, "rb");
    FILE *output_file = fopen(output_file_name, "wb");

    /* Verificação se foi possível abrir os arquivos */
    if (input_file == NULL || output_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    while (1) {
        /* Alocação de memória para armazenar provisoriamente o byte lido no arquivo */
        unsigned char *input_byte = (unsigned char *) malloc(sizeof(unsigned char));

        /* input_byte -> buffer de saída.
         * 1 -> Tamanho do bloco.
         * 1 -> Quantidade de blocos.
         * input_file -> Arquivo de entrada.
         * */
        if (fread(input_byte, 1, 1, input_file) != 1) {
            /* Libera a memória reservada para byte antes de sair do loop */
            free(input_byte);
            break;
        }

        /* Cada byte do arquivo será comparado com cada byte do dicionário. */
        for (int scan_dict = 0; scan_dict <= huffman_tree->tree_size / 2; scan_dict++) {
            /* Byte alvo presente no dicionário. */
            unsigned char target_byte =
                (unsigned char) huffman_tree->dictionary[scan_dict][0];

            /* "Byte" equivalente no dicionário. */
            target_eq_byte = huffman_tree->dictionary[scan_dict][1];

            /* Se o byte do arquivo for igual ao byte no dicionário. */
            if (target_byte == *input_byte) {
                /* A profundidade de um nó na árvore indica quantos bits compõem o "byte"
                 * equivalente. */
                unsigned long depth =
                    find_depth_in_huffman_tree(huffman_tree->linkedList, &target_byte, 0);

                /* Caso o tamanho da palavra de saída estoure o tamanho de 1 byte. */
                while (shift_output_cache + depth >= 8) {
                    /* shift_for_output -> quantidade de bits deslocados para a saída.
                     *
                     * shift_for_cache -> quantidade de bits deslocados para o cache.
                     */
                    unsigned int shift_for_cache = ((shift_output_cache + depth) - 8);

                    unsigned int shift_for_output = (depth - shift_for_cache);

                    /* Desloca os bits para a saída. */
                    output_byte = output_byte << shift_for_output;
                    output_byte = output_byte | (target_eq_byte >> shift_for_cache);

                    /* Escreve o byte no arquivo. */
                    size_t write =
                        fwrite(&output_byte, sizeof(output_byte), 1, output_file);

#if DEBUG_MODE
                    printf("########################## Output Byte 1: ");
                    printf_bit_by_bit(output_byte);
                    printf("\n");
#endif
                    /* Caso não seja possível gravar o byte no arquivo. */
                    if (write != 1) {
                        fclose(output_file);
                        return ERR_FILE_WRITE;
                    }

                    shift_output_cache = 0;
                    depth              = depth - shift_for_output;
                }

#if DEBUG_MODE
                printf("Pre-shift: ");
                printf_bit_by_bit(output_byte);
#endif

                /* Desloca o bit para a saída. */
                output_byte = output_byte << depth;
                output_byte = output_byte | target_eq_byte;
                shift_output_cache += depth;

#if DEBUG_MODE
                printf("shift cache: %d, depth: %lu -> ", shift_output_cache, depth);
                printf_bit_by_bit(output_byte);
                printf("\n");
#endif
                break;
            }
        }
    }

    /* Salva o último byte e calcula o lixo. */
    if (shift_output_cache) {
        trash_size  = 8 - shift_output_cache;
        output_byte = output_byte << trash_size;

        /* Escreve o byte no arquivo. */
        size_t write = fwrite(&output_byte, sizeof(output_byte), 1, output_file);

#if DEBUG_MODE
        printf("########################## Output Byte 2: ");
        printf_bit_by_bit(output_byte);
        printf("#####\n");
#endif

        /* Caso não seja possível gravar o byte no arquivo. */
        if (write != 1) {
            fclose(output_file);
            return ERR_FILE_WRITE;
        }

        huffman_tree->trash_size = trash_size;
    }

    /* Tamanho total do Header. */
    unsigned int size_head;

    /* Cria o cabeçalho e insere no arquivo. */
    header = make_header(huffman_tree, &size_head);

    fclose(input_file);
    fclose(output_file);

    insert_header(output_file_name, header, size_head);

    return 0;
}
