/**
 * @file test.c
 * @author fool_dog (2696652257@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../example/tlsf_impl.h"

void test_c()
{
    user_mem_init();
    show_user_heap_info();
    printf("c dynamic memory test---------------------\r\n");

    char *name = (char *)user_malloc(100);
    int *num = (int *)user_malloc(sizeof(int));
    char *buffer = (char *)user_malloc(20);
    float *array = (float *)user_malloc(sizeof(float) * 10);
    show_user_heap_info();

    user_free(num);
    user_free(buffer); // These two blocks of memory will be merged into one block.
    show_user_heap_info();

    user_free(name);
    user_free(array);
    show_user_heap_info();
}
