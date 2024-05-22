/**
 * @file user_mem_cpp.cpp
 * @author 独霸一方 (2696652257@qq.com)
 * @brief //> compile this file alongside tlsf.c and user_mem.c
 * @version 1.0
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <new>

#include "user_mem.h"

// clang-format off

#if defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
    #define THROW_BADALLOC throw(std::bad_alloc)
    #define THROW throw()
#else
    #define THROW_BADALLOC
    #define THROW
#endif

// clang-format on

//+******************************** overload std new/delete ***************************************/

/**
 * Please make sure you have called `user_mem_init` before using dynamic memory.
 * If you are using dynamic memory globally (outside functions),
 * make sure to open the `TLSF_INIT_BEFORE_MAIN` macro to ensure that dynamic memory is initialized before use.
 */

using namespace std;
void *operator new(std::size_t count) THROW_BADALLOC
{
    void *p = user_malloc(count);
    return p;
}

void *operator new(std::size_t count, const std::nothrow_t &tag) THROW
{
    (void)tag;
    void *p = user_malloc(count);
    return p;
}

void *operator new[](std::size_t count) THROW_BADALLOC
{
    void *p = user_malloc(count);
    return p;
}

void *operator new[](std::size_t count, const std::nothrow_t &tag) THROW
{
    (void)tag;
    void *p = user_malloc(count);
    return p;
}

void operator delete(void *ptr) THROW
{
    user_free(ptr);
}

void operator delete[](void *ptr) THROW
{
    user_free(ptr);
}

/* Provide function for pure virtual call to avoid huge demangling code being linked in ARM GCC */
#if ((defined(__GNUC__)) && (defined(__arm__)))
extern "C" void __cxa_pure_virtual(void)
{
    for (;;)
    {
    };
}
#endif
