/**
 * @file common.c
 *
 * @authors
 *
 * @brief
*/

#ifndef GZIP_V1_COMMON_H
#define GZIP_V1_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * @brief
 */
typedef struct byte_frequency {
    char byte;
    unsigned long frequency;
} byte_frequency_t;

/**
 * @brief
 */
typedef struct linked_list {
    void * data;
    struct linked_list * next;
    struct linked_list * right;
    struct linked_list * left;

    void (*func)(struct linked_list);
} linked_list_t;

/**
 * @brief
 * @return
 */
linked_list_t * create_empty_linked_list();

/**
 * @brief
 * @param head
 * @param data
 * @return
 */
int insert_at_beginning(linked_list_t **head, void *data);

/**
 * @brief
 * @param linked_list
 * @param target_char
 * @return
 */
int exist_in_linked_list(linked_list_t *linked_list, const char* target_char);

#endif //GZIP_V1_COMMON_H
