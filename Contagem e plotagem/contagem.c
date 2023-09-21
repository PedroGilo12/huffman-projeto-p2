#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Estrutura para representar um nó de Árvore Binária Degenerada.
 */
typedef struct node_degenerate {
    int data; /**< Dado armazenado no nó */
    struct node_degenerate* left; /**< Ponteiro para o filho esquerdo. */
}node_degenerate_t;

/**
 * @brief Estrutura para representar um nó da AVL.
 */
typedef struct node_avl {
    int data; /**< Dado armazenado no nó. */
    struct node_avl* left;  /**< Ponteiro para o filho esquerdo. */
    struct node_avl* right; /**< Ponteiro para o filho direito. */
    int height; /**< Altura do nó na árvore. */
}node_avl_t;

/**
 * @brief Aloca memória para um novo nó da Árvore Binária Degenerada e inicializa.
 *
 * @param data Valor a ser armazenado no nó.
 * @return Um ponteiro para o novo nó alocado.
 */
    node_degenerate_t* new_node_degenerate(int data) {
    
    node_degenerate_t* node = (node_degenerate_t*)malloc(sizeof(node_degenerate_t)); /**< Aloca memória para um novo nó */
    
    node->data = data; /**< Valor a ser armazenado no nó. */
    
    node->left = NULL; /**< Inicializa o ponteiro para o filho esquerdo como NULL */
    
    return node;
}

/**
 * @brief Aloca memória para um novo nó da AVL e inicializa. 
 *
 * @param data Valor a ser armazenado no nó.
 * @return Um ponteiro para o novo nó alocado.
 */
    node_avl_t* create_avl_node(int data) {

    node_avl_t* new_node = (node_avl_t*)malloc(sizeof(node_avl_t)); /**< Aloca memória para um novo nó */
    
    new_node->data = data; /**< Valor a ser armazenado no nó */
    
    new_node->left = NULL; /**< Inicializa o ponteiro para o filho esquerdo */
    
    new_node->right = NULL; /**< Inicializa o ponteiro para o filho direito */
    
    new_node->height = 1; /**< Inicializa a altura do nó como 1 */
    
    return new_node;
}


/**
 * @brief Insere um valor em uma Árvore Binária Degenerada.
 *
 * @param root Raiz da árvore.
 * @param data Elemento a ser inserido na árvore.
 * @return Um ponteiro para a raiz da árvore após a inserção.
 */
    node_degenerate_t* insert_degenerate(node_degenerate_t* root, int data) {
    
    if (root == NULL)  /**< Se a raiz for nula, cria um novo nó e retorna como a nova raiz */
        return new_node_degenerate(data);
    
    root->left = insert_degenerate(root->left, data); /**< Caso contrário, insere recursivamente o valor na subárvore esquerda */
    
    return root; /**< Retorna a raiz atualizada da árvore degenerada */
}


/**
 * @brief Função para calcular a altura de um nó da AVL.
 *
 * @param node Nó cuja altura será calculada.
 * @return A altura do nó.
 */
int get_height(node_avl_t* node) {
    
    if (node == NULL) /**<  Verifica se o nó é nulo, se for, a altura é 0 */
        return 0;
    
    return node->height; /**< Caso contrário, retorna a altura */
}

/**
 * @brief Função para calcular o fator de balanceamento de um nó da AVL.
 *
 * @param node Nó cujo fator de balanceamento será calculado.
 * @return O fator de balanceamento.
 */
int get_balance_factor(node_avl_t* node) {
    if (node == NULL) /**< Verifica se o nó é nulo, se for, o fator de balanceamento é 0 */
        return 0;
    
    return get_height(node->left) - get_height(node->right); /**< Caso contrário, retorna o fator de balanceamento  */
}

/**
 * @brief Função para atualizar a altura de um nó da AVL.
 *
 * @param node Nó cuja altura será atualizada.
 */
void update_height(node_avl_t* node) {
    
    if (node == NULL) /**< Verifica se o nó é nulo; se for, a altura é 0 */ 
        return;

    int leftHeight = get_height(node->left); /**< Obtém as alturas das subárvores esquerda */
    
    int rightHeight = get_height(node->right); /**< Obtém as alturas das subárvores a direita */

    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1; /**< Atualiza a altura do nó como a maior altura entre as subárvores + 1 */ 
}

/**
 * @brief Realiza uma rotação simples à direita em um nó da AVL.
 *
 * @param y O nó onde a rotação será realizada.
 * @return O novo nó raiz após a rotação.
 */
node_avl_t* rotate_right(node_avl_t* y) {
    
    node_avl_t* x = y->left; /**< Armazena o nó à esquerda de y em x */
    
    node_avl_t* T2 = x->right; /**< Armazena a subárvore à direita de x em T2 */

    /* Faz a rotação à direita, tornando `x` a nova raiz */
    x->right = y;
    y->left = T2;

    /* Atualiza as alturas dos nós afetados após a rotação */
    update_height(y);
    update_height(x);

    return x; /**< Retorna o novo nó raiz após a rotação */
}

/**
 * @brief Realiza uma rotação simples à esquerda em um nó da AVL.
 *
 * @param x O nó onde a rotação será realizada.
 * @return O novo nó raiz após a rotação.
 */
node_avl_t* rotate_left(node_avl_t* x) {
    
    node_avl_t* y = x->right; /**< Armazena o nó à direita de x em y */
    
    node_avl_t* T2 = y->left; /**< Armazena a subárvore à esquerda de y em T2 */

    /* Faz a rotação à esquerda, tornando y a nova raiz */
    y->left = x;
    x->right = T2;

    /* Atualiza as alturas dos nós afetados após a rotação */
    update_height(x);
    update_height(y);

    return y; /**< Retorna o novo nó raiz após a rotação */
}

/**
 * @brief Função para inserir um valor em uma AVL.
 *
 * @param root Raiz da árvore AVL.
 * @param data Elemento a ser inserido na árvore.
 * @return Um ponteiro para a raiz da árvore após a inserção.
 */
node_avl_t* insert_avl(node_avl_t* root, int data) {
    
    /* Cria um novo nó com o valor `data` se a árvore estiver vazia */
    if (root == NULL)
        return create_avl_node(data); 

    /* Insere o valor na subárvore esquerda se for menor */
    if (data < root->data)
        root->left = insert_avl(root->left, data);
    
    /* Insere o valor na subárvore direita se for maior */
    else if (data > root->data)
        root->right = insert_avl(root->right, data);

    /* Valores duplicados não são permitidos, retorna o nó atual */
    else
        return root; 

    
    update_height(root); /**< Atualiza a altura deste nó ancestral */

    int balance = get_balance_factor(root); /**< Verifica o fator de balanceamento deste nó para verificar se precisa de rotações */

    /* Caso de desequilíbrio à esquerda */
    if (balance > 1) {
        
        /* Rotação simples à direita */
        if (data < root->left->data) 
            return rotate_right(root); 
        
        /* Rotação dupla à esquerda-direita */
        else {
            root->left = rotate_left(root->left); /**< Realiza uma rotação à esquerda na subárvore esquerda */
            return rotate_right(root); /**< Realiza uma rotação simples à direita na raiz */
        }
    }

    /* Caso de desequilíbrio à direita */
    if (balance < -1) {

        /* Rotação simples à esquerda */
        if (data > root->right->data) 
            return rotate_left(root); 

        /* Rotação dupla à direita-esquerda */
         else {
            root->right = rotate_right(root->right); /**< Realiza uma rotação à direita na subárvore direita */ 
            return rotate_left(root); /**< Realiza uma rotação simples à esquerda na raiz */ 
        }
    }

    return root; /**< Retorna a raiz da árvore após a inserção */
}

/**
 * @brief Função para realizar uma busca em uma AVL e contar as comparações.
 *
 * @param root Raiz da árvore AVL.
 * @param target Valor a ser procurado na árvore.
 * @param comparisons Ponteiro para o contador de comparações.
 * @return 1 se o valor for encontrado, 0 caso contrário.
 */
int search_avl(node_avl_t* root, int target, int* comparisons) {
    
    /* Se a árvore estiver vazia, o valor não foi encontrado */
    if (root == NULL)
        return 0; 

    (*comparisons)++; /**< Incrementa o contador de comparações */ 

    /* Se o valor for igual ao valor do nó atual, o valor foi encontrado */
    if (root->data == target)
        return 1; 

    /* Recursivamente, busca na subárvore esquerda */
    else if (target < root->data)
        return search_avl(root->left, target, comparisons); 
    
    /* Recursivamente, busca na subárvore direita */
    else
        return search_avl(root->right, target, comparisons); 
}

/**
 * @brief Função para realizar uma busca em uma Árvore Binária Degenerada e contar as comparações.
 *
 * @param root Raiz da árvore Binária Degenerada.
 * @param target Valor a ser procurado na árvore.
 * @param comparisons Ponteiro para o contador de comparações.
 * @return 1 se o valor for encontrado, 0 caso contrário.
 */

    /* Se a árvore estiver vazia, o valor não foi encontrado */
    int search_degenerate(node_degenerate_t* root, int target, int* comparisons) {
    if (root == NULL)
        return 0; 

    (*comparisons)++; /**< Incrementa o contador de comparações */

    /* Se o valor for igual ao valor do nó atual, o valor foi encontrado */
    if (root->data == target)
        return 1; 

    /* Recursivamente, busca na subárvore esquerda */
    else if (target < root->data)
        return search_degenerate(root->left, target, comparisons); 
    
    /* Recursivamente, busca na subárvore esquerda (corrigindo um erro tipográfico, deveria ser direita) */
    else
        return search_degenerate(root->left, target, comparisons); 
}

/**
 * @brief Função para gerar números aleatórios diferentes.
 *
 * @param arr Array contendo números gerados anteriormente.
 * @param n Tamanho do array.
 * @param lower Valor mínimo dos números aleatórios.
 * @param upper Valor máximo dos números aleatórios.
 * @return Um número aleatório único.
 */
int generate_random_unique(int* arr, int n, int lower, int upper) {

    int num = rand() % (upper - lower + 1) + lower; /**< Gera um número aleatório no intervalo [lower, upper] */

    /* Entra em um loop infinito para garantir a geração de um número único */
    while (1) {
       
        int is_unique = 1;

        /*  Verifica se o número gerado já existe no array de números gerados anteriormente */
        for (int i = 0; i < n; i++) {
            
            if (arr[i] == num) {
                is_unique = 0; /**< Define isUnique como 0 se o número gerado já estiver no array */
                break;
            }
        }

        /* Se o número for único, retorna-o */
        if (is_unique)
            return num;

        /* Gera um novo número aleatório se o número não for único */
        num = rand() % (upper - lower + 1) + lower;
    }
}

int main() {
    
    srand(time(NULL)); /**< Inicializa a semente do gerador de números aleatórios */
    
    /* Abre os arquivo de saída */
    FILE* output_avl = fopen("output_avl", "w");
    FILE* output_degenerate = fopen("output_degenerate", "w"); 

    int lower = 1; /**< Valor mínimo dos números aleatórios */
    int upper = 20000; /**< Valor máximo dos números aleatórios */

    /* Loop para acrescentar na entrada */
    for (int n = 0; n <= 200000; n += 100) {
        
        /* Inicializa as árvores vazias */
        node_avl_t* avl_root = NULL;
        node_degenerate_t* degenerate_root = NULL;

        /* Inicializa um array para armazenar números gerados */
        int* random_numbers = (int*)malloc(n * sizeof(int));

        /* Gera e insere números aleatórios nas árvores */
        for (int i = 0; i < n; i++) {

            int num = generate_random_unique(random_numbers, i, lower, upper); /**< recebe o número gerado */
            
            random_numbers[i] = num; /**< insere o número gerado no array */

            avl_root = insert_avl(avl_root, num); /**< Insere na AVL */

            degenerate_root = insert_degenerate(degenerate_root, num); /**< Insere na árvore Binária Degenerada */
        }

        int target = -50; /**< Define o valor alvo que não está presente nos números gerados */

        /* Contadores de comparações */
        int avl_Comparisons = 0;
        int degenerate_comparisons = 0;

        search_avl(avl_root, target, &avl_Comparisons); /**< Busca o valor alvo na árvore AVL e conta as comparações */

        search_degenerate(degenerate_root, target, &degenerate_comparisons); /**< Pesquisa o valor alvo na árvore Binária Degenerada e conta as comparações */

        /* Escreve os resultados no arquivo de saída */
        fprintf(output_avl, "%d\t%d\n", n, avl_Comparisons);
        fprintf(output_degenerate, "%d\t%d\n", n, degenerate_comparisons);

        free(random_numbers); /**< Libera a memória alocada para o array de números aleatórios */
    }

    /* Fecha o arquivo de saída AVL */
    fclose(output_avl); 
    fclose(output_degenerate);

    return 0;
}
