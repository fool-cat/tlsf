/**
 * @file test_stress.cpp
 * @author fool_dog (2696652257@qq.com)
 * @brief you should compile `tlsf.c` `user_mem.c` in your project,don not compile the `user_mem_cpp.cpp`,this test use std::list it will use default new/delete
 * @version 1.0
 * @date 2024-05-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <chrono>
#include <ctime>
#include <list>
#include <stdint.h>
#include <time.h>

#include "../user_mem.h"

using namespace std;

// It is best to have the number of ticks or time in nanosecond precision.
#define GET_TIME() clock()
#define LOOP_TIMES (1e6)
#define EACH_SIZE_MAX (1 * 1024)
#define CRITICAL_SIZE (USER_POOL_SIZE * 1 / 2)

struct memInfo
{
    void *ptr;
    int size;
};

void test_stress_cpp()
{
    user_mem_init();
    show_user_heap_info();

    srand(time(0));
    uint32_t use_size = 0;
    int failed_count = 0;
    int success_count = 0;

    auto spend_time = GET_TIME();
    auto now_time = GET_TIME();
    spend_time = 0;

    list<memInfo> ptrs;

    for (int i = 0; i < LOOP_TIMES; i++)
    {
        int size = rand() % EACH_SIZE_MAX + 1;
        if (use_size + size > USER_POOL_SIZE)
        {
            // printf("第%d次内存分配失败,已经超过总大小,已分配内存大小: %d,本次分配大小: %d\n", i, use_size, size);
            i--;
            continue;
        }

        now_time = GET_TIME();
        void *ptr = user_malloc(size);
        spend_time += GET_TIME() - now_time;

        if (ptr != NULL)
        {
            use_size += size; // 实际上获取的内存是要比请求的大的
            ptrs.push_back({ptr, size});
            success_count++;
        }
        else
        {
            // cout << " 第" << i << "次内存分配失败,已分配内存大小: " << use_size << ",   本次分配大小:" << size << endl;
            failed_count++;
        }

        // 当内存总占用已经超过CRITICAL_SIZE,则随机释放一部分内存
        if (use_size >= CRITICAL_SIZE)
        {
            while (use_size >= CRITICAL_SIZE)
            {
                // 随机删除一个ptrs中的元素
                int index = rand() % ptrs.size();
                if (ptrs.size() > 0)
                {
                    auto it = ptrs.begin();
                    for (int j = 0; j < index; j++)
                    {
                        it = next(it);
                    }
                    // cout << "tlsf_free: " << it->second.size << " " << it->second.ptr << endl;
                    user_free(it->ptr);
                    use_size -= it->size;
                    ptrs.erase(it);
                    // 打印剩余map大小
                    // cout << "ptrs.size: " << ptrs.size() << endl;
                }
                else
                {
                    break;
                }
            }
        }
    }

    // 打印总计分配的内存大小
    cout << "use_size: " << use_size << endl;
    cout << "failed_count: " << failed_count << ",\tsuccess_count: " << success_count << endl;

    // 打印平均每条分配内存的时间
    if (failed_count + success_count > 0)
        cout << "average spend time: " << (double)spend_time / CLOCKS_PER_SEC * 1e6 / (failed_count + success_count) << endl; // maybe us?

    show_user_heap_info();
    for (auto it : ptrs)
    {
        user_free(it.ptr);
    }
    printf("free test used memory\r\n");
    show_user_heap_info();
}
