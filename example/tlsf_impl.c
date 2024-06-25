#include "tlsf_impl.h"
#include <string.h>

/*
** Set assert macro, if it has not been provided by the user.
*/
#if !defined(tlsf_assert)
#include <assert.h>
#define tlsf_assert assert
#endif

/*
** Set tlsf_printf macro, if it has not been provided by the user.
*/
#if !defined(tlsf_printf)
#include <stdio.h>
#define tlsf_printf printf
#endif

// tlsf句柄
static tlsf_t user_pool = NULL;

static char user_mem_block[USER_POOL_SIZE] __attribute__((aligned(sizeof(size_t)))); // TLSF requires memory alignment of 4/8 bytes on 32-bit/64-bit platforms.

#if TLSF_INIT_BEFORE_MAIN
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
	user_pool = tlsf_create_with_pool((void *)user_mem_block, sizeof(user_mem_block), USER_POOL_SIZE);
	USER_AOTMIC_EXIT();

	if (!user_pool)
	{
		tlsf_printf("tlsf_create_with_pool failed\r\n");
		tlsf_assert(user_pool != NULL);
		for (;;)
			; // hang here
	}
}

void *user_malloc(size_t size)
{
	tlsf_assert(user_pool != NULL);

	void *ptr = NULL;
	USER_AOTMIC_ENTER();
	ptr = tlsf_malloc(user_pool, size);
	USER_AOTMIC_EXIT();

	if (!ptr)
	{
		tlsf_printf("user_malloc failed, size = %zu", size);
	}

	return ptr;
}

void *user_realloc(void *ptr, size_t size)
{
	tlsf_assert(user_pool != NULL);

	void *newPtr = NULL;
	USER_AOTMIC_ENTER();
	newPtr = tlsf_realloc(user_pool, ptr, size);
	USER_AOTMIC_EXIT();
	return newPtr;
}

void *user_calloc(size_t num, size_t size)
{
	tlsf_assert(user_pool != NULL);

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
	tlsf_assert(user_pool != NULL);

	void *ptr = NULL;
	USER_AOTMIC_ENTER();
	ptr = tlsf_memalign(user_pool, align, size);
	USER_AOTMIC_EXIT();
	return ptr;
}

void user_free(void *ptr)
{
	tlsf_assert(user_pool != NULL);

	USER_AOTMIC_ENTER();
	tlsf_free(user_pool, ptr);
	USER_AOTMIC_EXIT();
}

//+******************************** test ***************************************/
static void my_walker(void *ptr, size_t size, int used, void *user)
{
	(void)user;
	tlsf_printf("\t%p %s size: %zu \r\n", ptr, used ? "used" : "free", size);
}

void show_user_heap_info(void)
{
	tlsf_assert(user_pool != NULL);

	tlsf_printf("---print user heap info:\r\n");
	tlsf_walk_pool(tlsf_get_pool(user_pool), my_walker, NULL);

	/*
	size_t tlsf_size(void);
	size_t tlsf_align_size(void);
	size_t tlsf_block_size_min(void);
	size_t tlsf_block_size_max(void);
	size_t tlsf_pool_overhead(void);
	size_t tlsf_alloc_overhead(void);
	*/
	tlsf_printf("tlsf heap info:\r\n");
	tlsf_printf("tlsf_size = :           %zu\r\n", tlsf_size(user_pool));
	tlsf_printf("tlsf_align_size = :     %zu\r\n", tlsf_align_size());
	tlsf_printf("tlsf_block_size_min = : %zu\r\n", tlsf_block_size_min());
	tlsf_printf("tlsf_block_size_max = : %zu\r\n", tlsf_block_size_max(user_pool));
	tlsf_printf("tlsf_pool_overhead = :  %zu\r\n", tlsf_pool_overhead());
	tlsf_printf("tlsf_alloc_overhead = : %zu\r\n", tlsf_alloc_overhead());
	tlsf_printf("\r\n");

	return;
}