// > 单次包含宏定义
#ifndef __TLSF_CONFIG_H_
#define __TLSF_CONFIG_H_

//+********************************* tlsf config **********************************/

/* close assert or replace with your own assert */
// #define tlsf_assert(expr) ((void)0)

/* close printf or replace with your own printf */
// #define tlsf_printf(...) ((void)0)
// #define tlsf_printf printf

/* force inline */
// #define tlsf_decl __attribute__((always_inline)) static __inline

//+********************************* example config template **********************************/

/* init the tlsf before main function */
#define TLSF_INIT_BEFORE_MAIN 0

/* Maximum memory block managed by tlsf */
#define TLSF_POOL_SIZE (16 * 1024) /* default 16KB */

/* usually use close/open irq to protect the tlsf operation */
#define TLSF_AOTMIC_ENTER() (void)0
#define TLSF_AOTMIC_EXIT() (void)0

/* example */
// #include "cmsis_compiler.h"
// #define TLSF_AOTMIC_ENTER() __disable_irq()
// #define TLSF_AOTMIC_EXIT() __enable_irq()

/* replace the global new/delete with tlsf_new/delete */
#define TLSF_GLOBAL_NEW 0

#endif //\ __TLSF_CONFIG_H_
