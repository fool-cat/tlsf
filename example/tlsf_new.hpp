/**
 * @file user_mem_cpp.cpp
 * @author 独霸一方 (2696652257@qq.com)
 * @brief //> no matter which time you use the dynamic memory, you need to ensure that the dynamic memory is initialized before use.
 * @version 1.0
 * @date 2024-05-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifdef __cplusplus

// > 单次包含宏定义
#ifndef __TLSF_NEW_H_
#define __TLSF_NEW_H_

#include <new>
#include <memory>

#include "tlsf_impl.h"

// clang-format off

#if defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
    #define THROW_BADALLOC throw(std::bad_alloc)
    #define THROW throw()
#else
    #define THROW_BADALLOC
    #define THROW
#endif

// clang-format on

//+******************************** base class for tlsf new/delete ***************************************/
class tlsf
{
public:
    static void *operator new(std::size_t count) THROW_BADALLOC
    {
        void *p = user_malloc(count);
        return p;
    }

    static void *operator new(std::size_t count, const std::nothrow_t &tag) THROW
    {
        (void)tag;
        void *p = user_malloc(count);
        return p;
    }

    static void *operator new[](std::size_t count) THROW_BADALLOC
    {
        void *p = user_malloc(count);
        return p;
    }

    static void *operator new[](std::size_t count, const std::nothrow_t &tag) THROW
    {
        (void)tag;
        void *p = user_malloc(count);
        return p;
    }

    static void operator delete(void *ptr) THROW
    {
        user_free(ptr);
    }

    static void operator delete[](void *ptr) THROW
    {
        user_free(ptr);
    }
};

//+******************************** alloctor for std container ***************************************/
template <typename T>
class tlsf_alloctor : public std::allocator<T>
{
public:
    tlsf_alloctor() noexcept {}

    template <typename U>
    tlsf_alloctor(const tlsf_alloctor<U> &) noexcept {}

    T *allocate(std::size_t n)
    {
        T *ptr = tlsf::operator new(n * sizeof(T));
        return ptr;
    }

    void deallocate(T *p, std::size_t n)
    {
        user_free(p);
    }
};

template <typename T, typename U>
bool operator==(const tlsf_alloctor<T> &, const tlsf_alloctor<U> &) { return true; }

template <typename T, typename U>
bool operator!=(const tlsf_alloctor<T> &, const tlsf_alloctor<U> &) { return false; }

#endif //\ __TLSF_NEW_H_

#endif //\ __cplusplus
