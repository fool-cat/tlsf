/**
 * @file main.c
 * @author fool_dog (2696652257@qq.com)
 * @brief C++ project test,you should compile `tlsf.c` `user_mem.c` in your project
 * @version 1.0
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPLACE_SYSTEM_MEM_FUNC // repalce std memory functions
#include "./user_mem.h"

int main()
{
    user_mem_init();
    show_user_heap_info();
    printf("c dynamic memory test---------------------\r\n");

    char *name = (char *)malloc(100);
    int *num = (int *)malloc(sizeof(int));
    char *buffer = (char *)malloc(20);
    float *array = (float *)malloc(sizeof(float) * 10);
    show_user_heap_info();

    free(num);
    free(buffer); // These two blocks of memory will be merged into one block.
    show_user_heap_info();

    free(name);
    free(array);
    show_user_heap_info();

    return 0;
}
