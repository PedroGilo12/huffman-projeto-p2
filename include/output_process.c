#include <common.h>

linked_list_t* create_tree_from_preorder(char *string, int *index)
{

    /* Se o item for um * ele vai criar um novo nó sempre antes de continuar a árvore. */
    if (string[*index] == '*')
    {
        linked_list_t * node = new_node(&string[*index]);
        (*index)++;
        node->left = (linked_list_t *) create_tree_from_preorder(string, index);
        node->right = (linked_list_t *) create_tree_from_preorder(string, index);
        return node;
    }
    else
    {
        /* Se o item for uma / ele vai pular ele e continuar a analisar os outros itens. */
        if(string[*index] == '/')
        {
            (*index)++;
        }

        linked_list_t * node = new_node(&string[*index]);
        (*index)++;

        return node;

    }

}

