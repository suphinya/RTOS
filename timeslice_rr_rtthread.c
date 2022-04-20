#include <rtthread.h>

#define THREAD_STACK_SIZE   1024
#define THREAD_PRIORITY     20
#define THREAD_TIMESLICE    10


static void thread_entry(void *parameter)
{
    rt_uint32_t count = 0;
    rt_uint32_t value;

    value = (rt_uint32_t)parameter;
    while (1)
    {
        if(0 == (count % 5))
                {
                    rt_kprintf("thread %d is running ,thread %d count = %d\n", value , value , count);

                    if(count> 200)
                        return;
                }
        count++;
    }
}


int main(void)
{
    static rt_thread_t thread_id1 = RT_NULL;
    static rt_thread_t thread_id2 = RT_NULL;

    // create thread
    // name , f , parameter , stack_size , priority , tick
    thread_id1 = rt_thread_create("thread1",
                                thread_entry,
                                (void*)1,
                                THREAD_STACK_SIZE,
                                THREAD_PRIORITY,
                                THREAD_TIMESLICE);
    if (thread_id1 != RT_NULL)
            rt_thread_startup(thread_id1);  // start thread

    thread_id2 = rt_thread_create("thread2",
                                    thread_entry,
                                    (void*)2,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY,
                                    THREAD_TIMESLICE-5);
    if (thread_id2 != RT_NULL)
                rt_thread_startup(thread_id2);  // start thread
    return 0;
}


