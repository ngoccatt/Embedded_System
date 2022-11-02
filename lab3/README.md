# HI, welcome to our Lab3

Well, u know, Vanilla FreeRTOS is design to work for single core, that's why their docs don't even mention them. But, in ESP32, we have 2 CORES, and with that, FreeRTOS has been modified by ESP team so that is can work more effective using 2 core of ESP32, which they usually call it (SMP - Symmetric Multiprocessing - https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/freertos-smp.html). That's is the reason why u won't see the behaviour that you're expecting when running FreeRTOS with ESP32.

And that's why, when we creating tasks, we PIN it to core 0. I don't know why, but pin in core 0 produce great result, while pin it to core 1 just produce weird output. Maybe core 1 is used for something that involve interrupt?

In this lab, we try to make some example for:

Preemtive Scheduling with Time slicing 
    Preemtive: Task that have higher priority will "preempt" task with lower priority every "tick" happen, when the task with higher priority is ready
    Time slicing: when 2 or more tasks that have the same priority, "round-robin" will be use so that these task will run concurrently ON ONE CORE. IF THERE'RE MULTIPLE CORE, it's gonna be a different story. OS will choose and let 2 of the task run in parallel, instead of doing round-robin.

Preemtive Scheduling without time slicing:
    No more roundrobin. If 2 or more task have the same priority, they'll have to wait for the other to complete (either yield or delay (blocked)) then they can join in the CPU to continue working.
    It's still preemtive. If there's a task with higher priority is ready, that task will preempt the current running task.

Cooperative Scheduling:
    No preemtive, no priority, no roundrobin. FCFS, I'm done u next.

## How to use 

### To run Preemtive with TimeSlicing, you have to modify 3 files:
    FreeRTOSConfig.h in include/esp_additions/freertos
        #define configUSE_PREEMPTION    1
    
    FreeRTOS.h in inlcude/freertos
        #define configUSE_TIME_SLICING    1

    Enable this line of code in main.c
        //for preemtive
    xTaskCreatePinnedToCore(&task2, "sync2", 2048, NULL, 1, NULL, 0);

### To run Preemtive without TimeSlicing, you have to modify 3 files:
    FreeRTOSConfig.h in include/esp_additions/freertos
        #define configUSE_PREEMPTION    1
    
    FreeRTOS.h in inlcude/freertos
        #define configUSE_TIME_SLICING    0

    Enable this line of code in main.c
        //for preemtive
    xTaskCreatePinnedToCore(&task2, "sync2", 2048, NULL, 1, NULL, 0);

### To run Cooperative, you have to modify 3 files:
    FreeRTOSConfig.h in include/esp_additions/freertos
        #define configUSE_PREEMPTION    0
    
    FreeRTOS.h in inlcude/freertos
        #define configUSE_TIME_SLICING    0

    Enable this line of code in main.c
         //for cooperative
    xTaskCreatePinnedToCore(&task2, "sync2", 2048, NULL, 2, NULL, 0);

## Result

Result of the test is place in the Result.xlsx file. I've replace the character "i" from task 1 with value "1", "o" with "2" and "x" with "3", then place them in excel to draw a graph. Since we only PIN these tasks to core 0 (NOT CORE 1), it run as expected from the document from vanilla FreeRTOS, which is designed to run on MCU with a single core.


