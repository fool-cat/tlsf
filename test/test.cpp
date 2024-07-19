/**
 * @file test.cpp
 * @author fool_dog (2696652257@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>

#include "../example/tlsf_new.hpp"

using namespace std;

// tlsf base class overload new/delete
class test : public tlsf
{
public:
    test() = default;
    test(int _a)
    {
        a = _a;
        printf("this a is %d-----\r\n", a);
    };
    ~test() = default;

    int a;
    float b;
};

// test *global = new test(1); // Global overloading (outside functions) requires opening the `TLSF_INIT_BEFORE_MAIN` macro.

void test_cpp()
{
    user_mem_init();
    show_user_heap_info();
    printf("cpp overload test---------------------\r\n");

#if TLSF_GLOBAL_NEW
    int32_t *g_int_p = new int32_t;
    int32_t *g_int_arr_p = new int32_t[10];
#endif

    test *p1 = new test;
    test *p2 = new test;
    test *p3 = new test(10);
    test *array_test = new test[10];
    show_user_heap_info();

#if TLSF_GLOBAL_NEW
    delete g_int_p;
    delete[] g_int_arr_p;
    show_user_heap_info();
#endif

    delete p2;
    delete p3;
    show_user_heap_info();

    delete p1;
    delete[] array_test;
    show_user_heap_info();
}
