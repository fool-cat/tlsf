#include <string.h>

#include "tlsf_config.h" // 自行保证路径可见性

#include "../tlsf.h"

#include "tlsf_impl.h"

// clang-format off

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

/* init the tlsf before main function */
#ifndef TLSF_INIT_BEFORE_MAIN
    #define TLSF_INIT_BEFORE_MAIN 0
#endif

/* Maximum memory block managed by tlsf */
#ifndef TLSF_POOL_SIZE
    #define TLSF_POOL_SIZE (16 * 1024) /* default 16KB */
#endif

/* usually use close/open irq to protect the tlsf operation */
#ifndef TLSF_AOTMIC_ENTER
    #define TLSF_AOTMIC_ENTER() (void)0
#endif

#ifndef TLSF_AOTMIC_EXIT
    #define TLSF_AOTMIC_EXIT() (void)0
#endif

// clang-format on

static tlsf_t user_pool = NULL;
static size_t user_mem_block[(TLSF_POOL_SIZE + sizeof(size_t) + 1) / sizeof(size_t)]; // TLSF requires memory alignment of 4/8 bytes on 32-bit/64-bit platforms.

//+*********************************  **********************************/

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

	TLSF_AOTMIC_ENTER();
	user_pool = tlsf_create_with_pool((void *)user_mem_block, sizeof(user_mem_block), sizeof(user_mem_block));
	TLSF_AOTMIC_EXIT();

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
	TLSF_AOTMIC_ENTER();
	ptr = tlsf_malloc(user_pool, size);
	TLSF_AOTMIC_EXIT();

#if 0
	if (!ptr)
	{
		tlsf_printf("user_malloc failed, size = %zu", size);
	}
#endif

	return ptr;
}

void *user_realloc(void *ptr, size_t size)
{
	tlsf_assert(user_pool != NULL);

	void *newPtr = NULL;
	TLSF_AOTMIC_ENTER();
	newPtr = tlsf_realloc(user_pool, ptr, size);
	TLSF_AOTMIC_EXIT();
	return newPtr;
}

void *user_calloc(size_t num, size_t size)
{
	tlsf_assert(user_pool != NULL);

	void *ptr = NULL;
	TLSF_AOTMIC_ENTER();
	ptr = tlsf_malloc(user_pool, num * size);
	TLSF_AOTMIC_EXIT();
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
	TLSF_AOTMIC_ENTER();
	ptr = tlsf_memalign(user_pool, align, size);
	TLSF_AOTMIC_EXIT();
	return ptr;
}

void user_free(void *ptr)
{
	tlsf_assert(user_pool != NULL);

	TLSF_AOTMIC_ENTER();
	tlsf_free(user_pool, ptr);
	TLSF_AOTMIC_EXIT();
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
