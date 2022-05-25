#include <Arduino_FreeRTOS.h> // library freertos

void setup() {
  Serial.begin(115200);
  Serial.println( "Demo time slice round robin" );

  // function , name , stack size , parameter , priority , handle
  xTaskCreate(thread_entry, "create thread 1", 128, (void*)1,2, NULL);
  xTaskCreate(thread_entry, "create thread 2", 128, (void*)2,2, NULL);
}

void loop() {}

void thread_entry(void *parameter) {
  uint32_t count = 0;
  int value = (int)parameter;
  while(1){
    if(0 == (count % 5)){
      Serial.println("thread "+ String(value) +" is running , thread"+ String(value) + " count = "+ String(count));
    }
    count += 1 ;

    if(id == 1){
      vTaskDelay(5); // delay 5 ticks
    }
    else{
      vTaskDelay(10);
    }
  }
}

