#include <Arduino_FreeRTOS.h> // library freertos
#include <semphr.h>

//  global variable ==> use for semaphore
SemaphoreHandle_t dynamic_sem ; 

void setup() {
  Serial.begin(115200);
  Serial.println( "Demo Semaphore" );

  dynamic_sem = xSemaphoreCreateBinary();
  if( dynamic_sem == NULL )
    {
      Serial.println("create dynamic semaphore failed.");
    }
    else
    {
      Serial.println("create done. dynamic semaphore value = 0.");
    }

  // create 3 task with same priority
  // priority thread 2 > thread 1
  xTaskCreate(rt_thread1_entry, "thread 1", 128,NULL,tskIDLE_PRIORITY+1, NULL);
  xTaskCreate(rt_thread2_entry, "thread 2", 128,NULL,tskIDLE_PRIORITY+2, NULL);  
}

void loop() {}

void rt_thread1_entry(){
  static int count = 0;

    while (1)
    {
        if (count <= 100)
        {
            count++;
        }
        else
            return;

        /* count release semaphore every 10 counts */
        if (0 == (count % 10))
        {
            Serial.println("release a dynamic semaphore.");
            xSemaphoreGive(dynamic_sem);
        }
    }

}

void rt_thread2_entry(){
  static int number = 0;
  while (1)
  {
    /* permanently wait for the semaphore; once obtain the semaphore, perform the number self-add operation */
    if (xSemaphoreTake(dynamic_sem, portMAX_DELAY) != pdTRUE) // like result in rtthread
    {
      Serial.println("thread2 take a semaphore, failed.");
      vSemaphoreDelete(dynamic_sem);
      return;
    }
    else
    {
      number++;
      Serial.println("take a dynamic semaphore. number = "+ String(number) );
    }
  }
}