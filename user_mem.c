/**
 * @file user_mem.c
 * @author 独霸一方 (2696652257@qq.com)
 * @brief //> compile this file alongside tlsf.c
 * @version 1.0
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "user_mem.h"
#include "tlsf.h"

// tlsf句柄
static tlsf_t user_pool = NULL;

static char user_mem_block[USER_POOL_SIZE] __attribute__((aligned(8))); // TLSF requires memory alignment of 4/8 bytes on 32-bit/64-bit platforms.

#ifdef TLSF_INIT_BEFORE_MAIN
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
    user_pool = tlsf_create_with_pool((void *)user_mem_block, USER_POOL_SIZE);
    USER_AOTMIC_EXIT();

    if (!user_pool)
    {
        printf("tlsf_create_with_pool failed\r\n");
        for (;;)
            ;
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

static void user_walker(void *ptr, size_t size, int used, void *user)
{
    (void)user;
    printf("\t%p %s size: %d (%p)\r\n", ptr, used ? "used" : "free", (unsigned int)size, block_from_ptr(ptr));
}

void show_user_heap_info(void)
{
    tlsf_walk_pool(tlsf_get_pool(user_pool), user_walker, NULL);

    /*
    size_t tlsf_size(void);
    size_t tlsf_align_size(void);
    size_t tlsf_block_size_min(void);
    size_t tlsf_block_size_max(void);
    size_t tlsf_pool_overhead(void);
    size_t tlsf_alloc_overhead(void);
    */
    printf("tlsf heap info:\r\n");
    printf("tlsf_size = :           %d\r\n", (unsigned int)tlsf_size());
    printf("tlsf_align_size = :     %d\r\n", (unsigned int)tlsf_align_size());
    printf("tlsf_block_size_min = : %d\r\n", (unsigned int)tlsf_block_size_min());
    printf("tlsf_block_size_max = : %d\r\n", (unsigned int)tlsf_block_size_max());
    printf("tlsf_pool_overhead = :  %d\r\n", (unsigned int)tlsf_pool_overhead());
    printf("tlsf_alloc_overhead = : %d\r\n", (unsigned int)tlsf_alloc_overhead());
    printf("\r\n");

    return;
}
