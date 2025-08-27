# Change

原版tlsf内存管理仅根据平台位数进行配置,对于MCU等小内存平台存在较多损耗,根据[LVGL-memory-reference](https://github.com/lvgl/lvgl/blob/master/src/stdlib/builtin/lv_tlsf.c),[ESP-tlsf-reference](https://github.com/espressif/tlsf)进行修改,可以动态根据要管理的最大内存块大小进行配置,比如STM32 F40*系列存在Core Coupled Memory（CCM）,这部分内存无法被DMA等访问,可以创建多个tlsf对象分别管理对应的内存池

The default TLSF control block size is determined based on your platform, typically 6KB+ for 64-bit systems and 3KB+ for 32-bit systems. When porting to certain embedded MCU platforms with limited RAM, where only tens of kilobytes of RAM are available, it may be necessary to reduce the control block size to save RAM. This can be achieved by adjusting the control block size automatically through macro definitions based on the maximum block size managed by TLSF.[LVGL-memory-reference](https://github.com/lvgl/lvgl/blob/master/src/stdlib/builtin/lv_tlsf.c)

# tlsf

Two-Level Segregated Fit memory allocator implementation.
Written by Matthew Conte (matt@baisoku.org).
Released under the BSD license.

Features
--------

* O(1) cost for malloc, free, realloc, memalign
* Extremely low overhead per allocation (4 bytes)
* ~Low overhead per TLSF management of pools (\~3kB)~ It depends on the total amount of memory that needs to be managed.
* Low fragmentation
* Compiles to only a few kB of code and data
* Support for adding and removing memory pool regions on the fly

Caveats
-------

* Currently, assumes architecture can make 4-byte aligned accesses
* Not designed to be thread safe; the user must provide this

Notes
-----

This code was based on the TLSF 1.4 spec and documentation found at:

    http://www.gii.upv.es/tlsf/main/docs

It also leverages the TLSF 2.0 improvement to shrink the per-block overhead from 8 to 4 bytes.

History
-------

2016/04/10 - v3.1

* Code moved to github
* tlsfbits.h rolled into tlsf.c
* License changed to BSD

2014/02/08 - v3.0

* This version is based on improvements from 3DInteractive GmbH
* Interface changed to allow more than one memory pool
* Separated pool handling from control structure (adding, removing, debugging)
* Control structure and pools can still be constructed in the same memory block
* Memory blocks for control structure and pools are checked for alignment
* Added functions to retrieve control structure size, alignment size, min and max block size, overhead of pool structure, and overhead of a single allocation
* Minimal Pool size is tlsf_block_size_min() + tlsf_pool_overhead()
* Pool must be empty when it is removed, in order to allow O(1) removal

2011/10/20 - v2.0

* 64-bit support
* More compiler intrinsics for ffs/fls
* ffs/fls verification during TLSF creation in debug builds

2008/04/04 - v1.9

* Add tlsf_heap_check, a heap integrity check
* Support a predefined tlsf_assert macro
* Fix realloc case where block should shrink; if adjacent block is in use, execution would go down the slow path

2007/02/08 - v1.8

* Fix for unnecessary reallocation in tlsf_realloc

2007/02/03 - v1.7

* tlsf_heap_walk takes a callback
* tlsf_realloc now returns NULL on failure
* tlsf_memalign optimization for 4-byte alignment
* Usage of size_t where appropriate

2006/11/21 - v1.6

* ffs/fls broken out into tlsfbits.h
* tlsf_overhead queries per-pool overhead

2006/11/07 - v1.5

* Smart realloc implementation
* Smart memalign implementation

2006/10/11 - v1.4

* Add some ffs/fls implementations
* Minor code footprint reduction

2006/09/14 - v1.3

* Profiling indicates heavy use of blocks of size 1-128, so implement small block handling
* Reduce pool overhead by about 1kb
* Reduce minimum block size from 32 to 12 bytes
* Realloc bug fix

2006/09/09 - v1.2

* Add tlsf_block_size
* Static assertion mechanism for invariants
* Minor bugfixes

2006/09/01 - v1.1

* Add tlsf_realloc
* Add tlsf_walk_heap

2006/08/25 - v1.0

* First release
