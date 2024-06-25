#include "tlsf_new.hpp"

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
