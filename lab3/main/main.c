#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "button.h"
#include <time.h>

//chinh macro o day khong co tac dung dau =))
// #define configUSE_PREEMPTION        0
// #define configUSE_TIME_SLICING      0


//this function help we know how many "i" is equal to 1s, and how many ticks happen in 1s
// to know how many tick happen in 1 s, u can use this function: 
// timeinMS * configTickRate (in menu config of freeRTOS) / 1000
/*
    if configTickRate is 100Hz, and we want to know that in 1000ms (timeInMS) is equal to how many ticks, then
    ticks = 1000 * 100 / 1000 = 100 tick
*/
void calculate_tick(void * pvParameter) {       
    int maxcount = pdMS_TO_TICKS(1000);
    int init_tick = 0;
    for(; ;) {
        init_tick = xTaskGetTickCount();
        printf("{");
        while (xTaskGetTickCount() - init_tick < maxcount) {
            printf("i");
        }
        printf("}\n");
        vTaskDelay(5000/ portTICK_RATE_MS);
        printf("%d", xTaskGetTickCount() - init_tick);
    }
    vTaskDelete(NULL);
}


void task1(void *pvParameter) {
    int maxcount = 5000;
    
    for(; ;) {
        int count = 0;
        printf("[");
        while (count < maxcount) {
            printf("i");
            count++;
        }
        printf("]");
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void task2(void * pvParameter) {
    int maxcount = 5000;
    
    for(; ;) {
        int count = 0;
        printf("{");
        while (count < maxcount) {
            printf("o");
            count++;
        }
        printf("}");
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void even_higher_priority_task(void* pvParameter) {
    int maxcount = 500;
    
    for(; ;) {
        int count = 0;
        printf("(");
        while (count < maxcount) {
            printf("x");
            count++;
        }
        printf(")");
        vTaskDelay(200 /portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}


void app_main(void)
{
    
    xTaskCreatePinnedToCore(&task1, "sync", 2048, NULL, 1, NULL, 0);

    //for preemtive
    xTaskCreatePinnedToCore(&task2, "sync2", 2048, NULL, 1, NULL, 0);
    //for cooperative
    // xTaskCreatePinnedToCore(&task2, "sync2", 2048, NULL, 2, NULL, 0);

    xTaskCreatePinnedToCore(&even_higher_priority_task, "high", 2048, NULL, 3, NULL, 0);

}