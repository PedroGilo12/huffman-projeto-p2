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

#include "locale.h"

/**
 * @brief Cria o nome do arquivo comprimido a partir do nome do arquivo original.
 * @param input_file_name [in] Nome do arquivo original.
 * @param output_file_name [out] Nome do arquivo comprimido.
 * @param gzip_flag Flag para indicar qual deve ser a extensão final do arquivo
 * comprimido.
 * @return 0 em caso de sucesso ou inteiro negativo em caso de falha.
 */
static int get_compress_file_names(char* input_file_name, char* output_file_name,
                                   char gzip_flag);

/**
 * @brief Interface para executar a operação de compressão.
 * @param input_file_name [in] Nome do arquivo que será comprimido.
 * @param output_file_name [in] Nome do arquivo comprimido.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
static int compress_interface(char* input_file_name, char* output_file_name);

/**
 * @brief Interface para executar a operação de extração.
 * @param input_file_name [in] Nome do arquivo comprimido.
 * @param output_file_name [in] Nome do arquivo descomprimido.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
static int extract_interface(char* input_file_name, char* output_file_name);

/**
 * @brief Escreve no terminal a logo do programa.
 */
static void logo_gzip(void);

/**
 * @brief Recupera o nome do arquivo descomprimido a partir do arquivo comprimido.
 * @param input_file_name [in] Nome do arquivo comprimido.
 * @param output_file_name [out] Nome do arquivo descomprimido.
 * @param ext Extensão do arquivo descomprimido.
 * @return 0 em caso de sucesso ou inteiro positivo em caso de falha.
 */
static int get_extracted_filename(char* input_file_name, char* output_file_name,
                                  char* ext);

void main_interface(void)
{
    /* Configura o terminal para suportar acentuação. */
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    char input_file_name[64];
    char output_file_name[64];
    char ext[64];
    int response      = -1;
    char gzip_flag    = 'x';
    char huffman_flag = ' ';
    bool execute      = 1;

    int err;

    logo_gzip();
    while (execute) {
        wprintf(L"Escolha uma das opções abaixo: \n");
        wprintf(L"1 - Compactar arquivo\n");
        wprintf(L"2 - Descompactar arquivo\n");
        wprintf(L"3 - Alternar extensão (%c).gip (%c).huff\n", gzip_flag, huffman_flag);
        wprintf(L"\n9 - Sair\n");

        wprintf(L"Digite a opção escolhida: ");
        scanf("%d", &response);

        switch (response) {
        case 1:
            printf("Digite o caminho do arquivo que deseja compactar: ");
            err = get_compress_file_names(input_file_name, output_file_name, gzip_flag);

            if(err != ERR_NONE) {
                printf("\033[2J\033[H");
                logo_gzip();
                wprintf(L"(Arquivo não encontrado)\n");
                continue;
            }

            printf("Arquivo de entrada: %s\nArquivo de saida: %s\n", input_file_name,
                   output_file_name);

            compress_interface(input_file_name, output_file_name);

            printf("\033[2J\033[H");
            logo_gzip();

            printf("(Arquivo de entrada: %s)\n(Arquivo de saida: %s)\n", input_file_name,
                   output_file_name);

            input_file_name[0]  = '\0';
            output_file_name[0] = '\0';

            break;
        case 2:
            while (1) {
                wprintf(L"Digite a extensão final do arquivo descompactado: ");
                scanf("%s", ext);

                if (ext[0] == '.') {
                    break;
                }
                wprintf(L"Extensão inválida, lembre-se de incluir o '.' no começo do "
                        L"nome da extensão.\n");
                ext[0] = '\0';
            }

            wprintf(L"Digite o caminho do arquivo que deseja extrair: ");
            get_extracted_filename(input_file_name, output_file_name, ext);

            printf("Arquivo de entrada: %s\nArquivo de saida: %s\n", input_file_name,
                   output_file_name);

            extract_interface(input_file_name, output_file_name);

            printf("\033[2J\033[H");
            logo_gzip();

            printf("(Arquivo de entrada: %s)\n(Arquivo de saida: %s)\n", input_file_name,
                   output_file_name);

            input_file_name[0]  = '\0';
            output_file_name[0] = '\0';

            break;
        case 3:
            printf("\033[2J\033[H");
            logo_gzip();
            if (gzip_flag == 'x') {
                gzip_flag    = ' ';
                huffman_flag = 'x';
                wprintf(L"(Extensão de arquivo alternada para .huff)\n");
            } else {
                gzip_flag    = 'x';
                huffman_flag = ' ';
                wprintf(L"(Extensão de arquivo alternada para .gip)\n");
            }
            break;

        case 9:
            execute = 0;
            break;
        default:
            printf("\033[2J\033[H");
            logo_gzip();
            wprintf(L"(Opção anterior escolhida inválida)\n");
            break;
        }
    }
}

static int get_compress_file_names(char* input_file_name, char* output_file_name,
                                   char gzip_flag)
{
    char* input = (char*) malloc(64 * sizeof(char));

    if (input == NULL) {
        return ERR_NULL_POINTER;
    }

    scanf("%s", input_file_name);
    strcpy(input, input_file_name);

    FILE * input_file = fopen(input_file_name, "r");

    if(input_file == NULL) {
        return ERR_FILE_NOT_FOUND;
    }

    fclose(input_file);

    for (int index = 0; index < strlen(input_file_name); index++) {
        if (input_file_name[index] == '.') {
            input_file_name[index] = '\0';
        }
    }

    char* input_cache = (char*) malloc(64 * sizeof(char));
    strcpy(input_cache, input_file_name);

    if (gzip_flag == 'x') {
        strcat(input_file_name, ".gip");
    } else {
        strcat(input_file_name, ".gip");
    }


    for (int attempts = 1; attempts < N_ATTEMPTS_TO_OPEN_FILE; attempts++) {
        if (fopen(input_file_name, "r") == NULL) {
            break;
        }

        char* suffix = (char*) malloc(4 * sizeof(char));
        sprintf(suffix, "(%d)", attempts);
        strcpy(input_file_name, input_cache);
        strcat(input_file_name, suffix);
        strcat(input_file_name, ".gip");
    }

    FILE* output_file = fopen(input_file_name, "w");

    if(output_file == NULL) {
        return ERR_FILE_WRITE;
    }

    strcpy(output_file_name, input_file_name);
    strcpy(input_file_name, input);
    fclose(output_file);

    return 0;
}

static int compress_interface(char* input_file_name, char* output_file_name)
{
    /* Cria uma lista encadeada vazia. */
    linked_list_t* linked_list = create_empty_linked_list();

    /* Transfere o arquivo de entrada como frequencias de bytes para a lista vazia. */
    ELAPSED_TIME("[PROCESS INPUT]",
                 process_input_file_as_byte_frequency(input_file_name, &linked_list));

    /* Ordena a lista pela frequência. */
    ELAPSED_TIME("[SORT_INPUT]", sort_linked_list_by_frequency(&linked_list));

    // print_linked_list_byte_frequency(linked_list);

    /* Monta a árvore de huffman a partir da lista anterior. */
    huffman_tree_t* huffman_tree = make_huffman_tree(&linked_list);

    /* Zera as váriaveis estaticas. */
    make_preorder_dictionary(STATIC_RESET);

    /* Cria o dicionário para a árvore. */
    ELAPSED_TIME("[MAKE_PREORDER]", make_preorder_dictionary(&huffman_tree->linkedList,
                                                             &huffman_tree->dictionary,
                                                             &huffman_tree->preorder, 0));

    /* Compacta o arquivo. */
    ELAPSED_TIME("[COMPRESS_FILE]",
                 compress_file(input_file_name, output_file_name, huffman_tree));

#if TRACE_MODE
    wprintf(L"Trace mode habilitado, digite 1 para continuar:\n");
    int press;
    scanf("%d", &press);
#endif
    return 0;
}

static int extract_interface(char* input_file_name, char* output_file_name)
{
    file_header_t* header = (file_header_t*) malloc(sizeof(file_header_t));

    ELAPSED_TIME("[GET_HEADER]", get_header_from_file(input_file_name, &header));

    int index = 0;

    huffman_tree_t* huffman_output = (huffman_tree_t*) malloc(sizeof(huffman_tree_t));

    int qtd_caracter_scape = 0;

    ELAPSED_TIME("[CREATE_TREE]",
                 huffman_output->linkedList = create_tree_from_preorder(
                     header->preorder_tree, &index, &qtd_caracter_scape));

    huffman_output->preorder =
        (unsigned char*) malloc(huffman_output->tree_size * sizeof(char));
    huffman_output->tree_size  = header->tree_size;
    huffman_output->trash_size = header->trash_size;
    huffman_output->preorder   = header->preorder_tree;

    huffman_output->dictionary = (unsigned long**) malloc(
        ((huffman_output->tree_size / 2)) * sizeof(unsigned long*));

    make_preorder_dictionary(STATIC_RESET);

    ELAPSED_TIME(
        "[MAKE_PREORDER]",
        make_preorder_dictionary(&huffman_output->linkedList, &huffman_output->dictionary,
                                 &huffman_output->preorder, 0);)

    ELAPSED_TIME("[EXTRACT_FILE]", extract_file(input_file_name, output_file_name,
                                                huffman_output, qtd_caracter_scape);)

#if TRACE_MODE
    wprintf(L"Trace mode habilitado, digite 1 para continuar:\n");
    int press;
    scanf("%d", &press);
#endif
    return 0;
}

static int get_extracted_filename(char* input_file_name, char* output_file_name,
                                  char* ext)
{
    char* input = (char*) malloc(64 * sizeof(char));

    if (input == NULL) {
        return ERR_NULL_POINTER;
    }

    scanf("%s", input_file_name);
    strcpy(input, input_file_name);

    for (int index = 0; index < strlen(input_file_name); index++) {
        if (input_file_name[index] == '.') {
            input_file_name[index] = '_';
        }
    }

    strcat(input_file_name, ext);

    printf("Nome do arquivo descompactado: %s\n", input_file_name);

    strcpy(output_file_name, input_file_name);
    strcpy(input_file_name, input);

    return 0;
}

static void logo_gzip(void)
{
    printf(
        "###########################################################################\n");
    printf(
        "##                                                                       ##\n");
    printf(
        "##  _______   _______   ___    _______                                   ##\n");
    printf(
        "## |       | |       | |   |  |       |                                  ##\n");
    printf(
        "## |    ___| |____   | |   |  |    _  |                                  ##\n");
    printf(
        "## |   | __   ____|  | |   |  |   |_| |                                  ##\n");
    printf(
        "## |   ||  | | ______| |   |  |    ___|                                  ##\n");
    printf(
        "## |   |_| | | |_____  |   |  |   |                                      ##\n");
    printf(
        "## |_______| |_______| |___|__|___|______   __   __   _______   __    _  ##\n");
    printf(
        "## |  | |  | |  | |  | |       | |       | |  |_|  | |   _   | |  |  | | ##\n");
    printf(
        "## |  |_|  | |  | |  | |    ___| |    ___| |       | |  |_|  | |   |_| | ##\n");
    printf(
        "## |       | |  |_|  | |   |___  |   |___  |       | |       | |       | ##\n");
    printf(
        "## |       | |       | |    ___| |    ___| |       | |       | |  _    | ##\n");
    printf(
        "## |   _   | |       | |   |     |   |     | ||_|| | |   _   | | | |   | ##\n");
    printf(
        "## |__| |__| |_______| |___|     |___|     |_|   |_| |__| |__| |_|  |__| ##\n");
    printf(
        "##                                                                       ##\n");
    printf(
        "###########################################################################\n");
}