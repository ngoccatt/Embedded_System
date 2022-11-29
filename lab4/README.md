# Error and Experience #

## FreeRTOS Task should not return - ESP32 ## 
https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32
A task should be ended with vTaskDelete(NULL);

 ## A MCU have limited memory ## 
If you continously create a new queue entry (malloc some space for it) without delete (free), then, there will be time when it's out of memory. ESP will trigger restart or raise exception if there's no more memory available (StoreProhibited?)
Fatal Error: LoadProhibited or StoreProhibited, first check for registor EXCVADDR, if it's 0, then NULL reference must happen somewhere
EXCVADDR: 0x0000000

 ## ESP Fatal Error List ## 
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/fatal-errors.html?highlight=storeprohibited

## Run Result ##
pump: received LED, but it's not my task  
fan: received LED, but it's not my task  
pump: received LED, but it's not my task  
LED  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
PUMP  
pump: received LED, but it's not my task  
LED  
pump: received LED, but it's not my task  
LED  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
led: received PUMP, but it's not my task  
fan: received PUMP, but it's not my task  
PUMP  
led: received PUMP, but it's not my task  
fan: received PUMP, but it's not my task  
PUMP  
led: received FAN, but it's not my task  
FAN  
LED  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
fan: received LED, but it's not my task  
pump: received LED, but it's not my task  
LED  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
pump: received LED, but it's not my task  
LED  
fan: received PUMP, but it's not my task  
PUMP  
fan: received noob, but it's not my task  
pump: received noob, but it's not my task  
led: received noob, but it's not my task  
fan: received noob, but it's not my task  
This task noob is rejected 3 times, skiping the task  
pump: received LED, but it's not my task  
LED  
pump: received LED, but it's not my task  
LED  
pump: received FAN, but it's not my task  
led: received FAN, but it's not my task  
