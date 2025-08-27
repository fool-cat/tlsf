#ifndef INCLUDED_tlsf
#define INCLUDED_tlsf

/*
** Two Level Segregated Fit memory allocator, version 3.1.
** Written by Matthew Conte
**	http://tlsf.baisoku.org
**
** Based on the original documentation by Miguel Masmano:
**	http://www.gii.upv.es/tlsf/main/docs
**
** This implementation was written to the specification
** of the document, therefore no GPL restrictions apply.
**
** Copyright (c) 2006-2016, Matthew Conte
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the copyright holder nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL MATTHEW CONTE BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    /* tlsf_t: a TLSF structure. Can contain 1 to N pools. */
    /* pool_t: a block of memory that TLSF can manage. */
    typedef void *tlsf_t;
    typedef void *pool_t;

    /* Create/destroy a memory pool. */
    tlsf_t tlsf_create(void *mem, size_t mem_size, size_t max_pool_size); /* TLSF can manage 1 to N non-contiguous memory pools, where max_pool_size represents the size of the largest pool among them. */
    tlsf_t tlsf_create_with_pool(void *mem, size_t mem_size, size_t max_pool_size);
    void tlsf_destroy(tlsf_t tlsf);
    pool_t tlsf_get_pool(tlsf_t tlsf);

    /* Add/remove memory pools. */
    pool_t tlsf_add_pool(tlsf_t tlsf, void *mem, size_t bytes);
    void tlsf_remove_pool(tlsf_t tlsf, pool_t pool);

    /* malloc/memalign/realloc/free replacements. */
    void *tlsf_malloc(tlsf_t tlsf, size_t bytes);
    void *tlsf_memalign(tlsf_t tlsf, size_t align, size_t bytes);
    void *tlsf_realloc(tlsf_t tlsf, void *ptr, size_t size);
    void tlsf_free(tlsf_t tlsf, void *ptr);

    /* Returns internal block size, not original request size */
    size_t tlsf_block_size(void *ptr);

    /* Overheads/limits of internal structures. */
    size_t tlsf_size(tlsf_t tlsf);
    size_t tlsf_align_size(void);
    size_t tlsf_block_size_min(void);
    size_t tlsf_block_size_max(tlsf_t tlsf); /* The actual maximum memory will be slightly less than the max_pool_size. */
    size_t tlsf_pool_overhead(void);
    size_t tlsf_alloc_overhead(void);

    /* Debugging. */
    typedef void (*tlsf_walker)(void *ptr, size_t size, int used, void *user);
    void tlsf_walk_pool(pool_t pool, tlsf_walker walker, void *user);
    /* Returns nonzero if any internal consistency check fails. */
    int tlsf_check(tlsf_t tlsf);
    int tlsf_check_pool(pool_t pool);

#if defined(__cplusplus)
};
#endif

// Compile-time calculation of the specified TLSF control block size.

// clang-format off

#if defined (__alpha__) || defined (__ia64__) || defined (__x86_64__) \
	|| defined (_WIN64) || defined (__LP64__) || defined (__LLP64__)
    #if !defined TLSF_64BIT
        #define TLSF_64BIT
    #endif
#endif

#define TLSF_FLS1(n)  ((n) & 0x1        ?  1 : 0)
#define TLSF_FLS2(n)  ((n) & 0x2        ?  1 + TLSF_FLS1 ((n) >>  1) : TLSF_FLS1 (n))
#define TLSF_FLS4(n)  ((n) & 0xc        ?  2 + TLSF_FLS2 ((n) >>  2) : TLSF_FLS2 (n))
#define TLSF_FLS8(n)  ((n) & 0xf0       ?  4 + TLSF_FLS4 ((n) >>  4) : TLSF_FLS4 (n))
#define TLSF_FLS16(n) ((n) & 0xff00     ?  8 + TLSF_FLS8 ((n) >>  8) : TLSF_FLS8 (n))
#define TLSF_FLS32(n) ((n) & 0xffff0000 ? 16 + TLSF_FLS16((n) >> 16) : TLSF_FLS16(n))

#ifdef TLSF_64BIT
    #define TLSF_FLS(n) ((n) & 0xffffffff00000000ull ? 32 + TLSF_FLS32(((size_t)(n) >> 32)& 0xffffffff) : TLSF_FLS32(n))
#else
    #define TLSF_FLS(n) TLSF_FLS32(n)
#endif

/*
** Returns round up value of log2(n).
** Note: it is used at compile time.
*/
#define TLSF_LOG2_CEIL(n) ((n) & (n - 1) ? TLSF_FLS(n) : TLSF_FLS(n) - 1)

// clang-format on

#define CALC_FL_INDEX_MAX(max_pool_size) \
    TLSF_LOG2_CEIL(max_pool_size)

#define CALC_SL_INDEX_COUNT_LOG2(max_pool_size)                                   \
    (((max_pool_size) <= (16 * 1024)) ? 3 : ((max_pool_size) <= (256 * 1024)) ? 4 \
                                                                              : 5)

#define CALC_SL_INDEX_COUNT(max_pool_size) \
    (1 << CALC_SL_INDEX_COUNT_LOG2(max_pool_size))

#if defined(TLSF_64BIT)
#define CALC_FL_INDEX_SHIFT(max_pool_size) \
    (CALC_SL_INDEX_COUNT_LOG2(max_pool_size) + 3)
#else
#define CALC_FL_INDEX_SHIFT(max_pool_size) \
    (CALC_SL_INDEX_COUNT_LOG2(max_pool_size) + 2)
#endif

#define CALC_FL_INDEX_COUNT(max_pool_size) \
    (CALC_FL_INDEX_MAX(max_pool_size) - CALC_FL_INDEX_SHIFT(max_pool_size) + 1)

extern const size_t control_size;
#define TLSF_CALC_SZIE(max_pool_size)                                         \
    (control_size + (sizeof(size_t *) * CALC_FL_INDEX_COUNT(max_pool_size)) + \
     (sizeof(size_t *) * (CALC_FL_INDEX_COUNT(max_pool_size) * CALC_SL_INDEX_COUNT(max_pool_size))))

// #define _DEBUG 1

#endif
