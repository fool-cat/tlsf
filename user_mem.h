/**
 * @file user_mem.h
 * @author fool_dog (2696652257@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-02-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _USER_MEM_H_
#define _USER_MEM_H_

// include your header file

// clang-format off

/* init the tlsf before main */
#ifndef TLSF_INIT_BEFORE_MAIN
    #define TLSF_INIT_BEFORE_MAIN 0
#endif

/*  replace system memory function */
#ifndef REPLACE_SYSTEM_MEM_FUNC
    #define REPLACE_SYSTEM_MEM_FUNC 0
#endif

/* Maximum memory block managed by tlsf */
#ifndef USER_POOL_SIZE
    #define USER_POOL_SIZE (16 * 1024) /* default 16KB */
#endif

/* The maximum amount of memory that TLSF can manage.  */
#ifndef TLSF_MAX_POOL_SIZE
    #define TLSF_MAX_POOL_SIZE USER_POOL_SIZE
#endif

/* usually use close/open irq to protect the tlsf operation */
#ifndef USER_AOTMIC_ENTER
    #define USER_AOTMIC_ENTER() (void)0
#endif

#ifndef USER_AOTMIC_EXIT
    #define USER_AOTMIC_EXIT() (void)0
#endif

/* example */
// #include "cmsis_compiler.h"
// #define USER_AOTMIC_ENTER() __disable_irq()
// #define USER_AOTMIC_EXIT() __enable_irq()

// clang-format on

#ifdef __cplusplus
extern "C"
{
#endif

    void user_mem_init(void);

    void *user_malloc(size_t size);
    void *user_realloc(void *ptr, size_t size);
    void *user_calloc(size_t num, size_t size);
    void *user_memalign(size_t align, size_t size);
    void user_free(void *ptr);

    // print info
    void show_user_heap_info(void);

#ifdef __cplusplus
}
#endif //\ __cplusplus

// clang-format off

/* You need to include this header file at the beginning of the file that you want to replace. */
#if REPLACE_SYSTEM_MEM_FUNC
    #undef malloc
    #undef realloc
    #undef calloc
    #undef memalign
    #undef free

    #define malloc      user_malloc
    #define realloc     user_realloc
    #define calloc      user_calloc
    #define memalign    user_memalign
    #define free        user_free
#endif

// clang-format on

#endif //\ _USER_MEM_H_
