#ifndef _TLSF_USER_CONFIG_H_
#define _TLSF_USER_CONFIG_H_

// clang-format off

/* init the tlsf before main function */
#ifndef TLSF_INIT_BEFORE_MAIN
    #define TLSF_INIT_BEFORE_MAIN 0
#endif

/* Maximum memory block managed by tlsf */
#ifndef USER_POOL_SIZE
    #define USER_POOL_SIZE (16 * 1024) /* default 16KB */
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

/*  replace system memory function */
#ifndef REPLACE_SYSTEM_MEM_FUNC
    #define REPLACE_SYSTEM_MEM_FUNC 0
#endif

#ifdef __cplusplus
    //+******************************** replace the global new/delete with tlsf_new/delete ***************************************/
    #ifndef TLSF_GLOBAL_NEW
        #define TLSF_GLOBAL_NEW 0
    #endif // TLSF_GLOBAL_NEW
#endif //\ __cplusplus

// clang-format on

#endif //\ _TLSF_USER_CONFIG_H_