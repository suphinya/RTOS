#include <rtthread.h>

static rt_thread_t thread_one = RT_NULL;
static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        rt_kprintf("thread1 count: %d\n", count ++);
        rt_thread_mdelay(500); // delay 500 millisec
    }
}

int main(void)
{
    // create thread
    // name , f , parameter , stack_size , priority , tick
    thread_one = rt_thread_create("thread1",
                                thread1_entry,
                                RT_NULL,
                                512,
                                25,
                                5);

    rt_thread_startup(thread_one); // start thread
    return 0;
}

