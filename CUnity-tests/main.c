#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "../include/common.h"
#include "../include/compress_process.h"

#define CU_ADD_TEST_PRINT_FUNC(suite, func) CU_add_test(suite, #func, func);

linked_list_t *linked_list;

int inicializar_suite(void)
{
    return 0;  // sucesso
}

int clear_suite(void)
{
    return 0;  // sucesso
}

void create_empty_list(void)
{
    CU_ASSERT_NOT_EQUAL(create_empty_linked_list(), NULL);
}

void insert_byte(void)
{
    linked_list = create_empty_linked_list();
    CU_ASSERT_PTR_NOT_NULL(linked_list);

    for (int byte = 0; byte < 100; byte++) {
        CU_ASSERT_EQUAL(insert_byte_frequency_at_beginning(&linked_list, &byte, 1), 0);
    }
}

void sort_linked_list(void)
{
    CU_ASSERT_EQUAL(sort_linked_list_by_frequency(&linked_list), 0);
}

void create_new_node(void)
{
    for (int byte = 0; byte < 256; byte++) {
        CU_ASSERT_NOT_EQUAL(new_node, NULL);
    }
}

void insert_ordered(void)
{
    byte_frequency_t byte_frequency = {
        .byte      = '*',
        .frequency = 2,
    };

    for (int frequency = 101; frequency < 256; frequency++) {
        byte_frequency.frequency = frequency;
        byte_frequency.byte      = frequency;

        linked_list_t new_node = {
            .data  = (byte_frequency_t *) &byte_frequency,
            .left  = NULL,
            .right = NULL,
            .func  = NULL,
            .next = NULL,
        };

        CU_ASSERT_EQUAL(insert_ordered_in_linked_list(&linked_list, &new_node), 0);
    }
}

void make_huffman_tree_(void)
{
    //huffman_tree_t * huffman_tree = make_huffman_tree(&linked_list);
    //CU_ASSERT_PTR_NOT_NULL(linked_list);
}

int main()
{
    // Inicializa o registro de suíte de testes do CUnit
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Adiciona uma suíte de testes ao registro
    CU_pSuite common_functions =
        CU_add_suite("Test common functions", inicializar_suite, clear_suite);
    // Adiciona os testes à suíte
    CU_ADD_TEST_PRINT_FUNC(common_functions, create_new_node);
    CU_ADD_TEST_PRINT_FUNC(common_functions, create_empty_list);
    CU_ADD_TEST_PRINT_FUNC(common_functions, insert_byte);
    CU_ADD_TEST_PRINT_FUNC(common_functions, sort_linked_list);
    CU_ADD_TEST_PRINT_FUNC(common_functions, insert_ordered);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
