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

#include <stddef.h>
#include <stdint.h>

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

#endif //\ _USER_MEM_H_
