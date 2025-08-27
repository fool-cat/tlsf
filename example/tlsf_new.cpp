#include "tlsf_config.h" // 自行保证路径可见性

#include "tlsf_new.hpp"
#include "tlsf_impl.h"

#ifndef TLSF_GLOBAL_NEW
#define TLSF_GLOBAL_NEW 0
#endif // TLSF_GLOBAL_NEW

#if TLSF_GLOBAL_NEW

//+******************************** overload global std new/delete ***************************************/
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

#endif

/* Provide function for pure virtual call to avoid huge demangling code being linked in ARM GCC */
#if ((defined(__GNUC__)) && (defined(__arm__)))
extern "C" void __cxa_pure_virtual(void)
{
    for (;;)
    {
    };
}
#endif
