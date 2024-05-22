/**
 * @file user_mem.c
 * @author 独霸一方 (2696652257@qq.com)
 * @brief //> Only you need to include this file to compile is enough when your project is a C project.
 * @version 1.0
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */

// user_mem.h should be included before tlsf.h
#include "user_mem.h"

#include "tlsf.h"
// include source code of tlsf.c
#include "tlsf.c"

// tlsf句柄
static tlsf_t user_pool = NULL;

static char user_mem_block[TLSF_POOL_SIZE] __attribute__((aligned(8))); // TLSF requires memory alignment of 4/8 bytes on 32-bit/64-bit platforms.

#ifdef INIT_BEFORE_MAIN
__attribute__((constructor)) static void _SystemStart(void)
{
    user_mem_init();
}
#endif

void user_mem_init(void)
{
    if (user_pool)
        return;

    USER_AOTMIC_ENTER();
    user_pool = tlsf_create_with_pool((void *)user_mem_block, TLSF_POOL_SIZE);
    USER_AOTMIC_EXIT();

    if (!user_pool)
    {
        while (1)
        {
            printf("tlsf_create_with_pool failed\n");
        }
    }
}

void *user_malloc(size_t size)
{
    void *ptr = NULL;
    USER_AOTMIC_ENTER();
    ptr = tlsf_malloc(user_pool, size);
    USER_AOTMIC_EXIT();

    if (!ptr)
    {
        printf("user_malloc failed, size = %d", size);
    }

    return ptr;
}

void *user_realloc(void *ptr, size_t size)
{
    void *newPtr = NULL;
    USER_AOTMIC_ENTER();
    newPtr = tlsf_realloc(user_pool, ptr, size);
    USER_AOTMIC_EXIT();
    return newPtr;
}

void *user_calloc(size_t num, size_t size)
{
    void *ptr = NULL;
    USER_AOTMIC_ENTER();
    ptr = tlsf_malloc(user_pool, num * size);
    USER_AOTMIC_EXIT();
    if (ptr)
    {
        memset(ptr, 0, num * size);
    }
    return ptr;
}

void *user_memalign(size_t align, size_t size)
{
    void *ptr = NULL;
    USER_AOTMIC_ENTER();
    ptr = tlsf_memalign(user_pool, align, size);
    USER_AOTMIC_EXIT();
    return ptr;
}

void user_free(void *ptr)
{
    USER_AOTMIC_ENTER();
    tlsf_free(user_pool, ptr);
    USER_AOTMIC_EXIT();
}

//+******************************** test ***************************************/

void show_user_heap_info(void)
{
    tlsf_walk_pool(tlsf_get_pool(user_pool), NULL, NULL);

    /*
    size_t tlsf_size(void);
    size_t tlsf_align_size(void);
    size_t tlsf_block_size_min(void);
    size_t tlsf_block_size_max(void);
    size_t tlsf_pool_overhead(void);
    size_t tlsf_alloc_overhead(void);
    */
    printf("tlsf heap info:");
    printf("tlsf_size = :           %d\r\n", tlsf_size());
    printf("tlsf_align_size = :     %d\r\n", tlsf_align_size());
    printf("tlsf_block_size_min = : %d\r\n", tlsf_block_size_min());
    printf("tlsf_block_size_max = : %d\r\n", tlsf_block_size_max());
    printf("tlsf_pool_overhead = :  %d\r\n", tlsf_pool_overhead());
    printf("tlsf_alloc_overhead = : %d\r\n", tlsf_alloc_overhead());

    return;
}
