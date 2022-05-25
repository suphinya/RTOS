#include <Arduino_FreeRTOS.h> // library freertos

void setup() {
  Serial.begin(115200);
  Serial.println( "Arduino Demo" );

  // function , name , stack size , parameter , priority , handle
  xTaskCreate(thread1_create, "create thread 1", 128, NULL,1, NULL);
}

void loop() {}

void thread1_create() {
  uint32_t count = 0;
  while(1){
    Serial.println("thread1 count: "+ String(count));
    vTaskDelay(pdMS_TO_TICKS(500)); // 500 millisec
    count += 1 ;
  }
}
