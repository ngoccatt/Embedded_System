#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "freertos/timers.h"
#include <time.h>

#define LOOPLIMIT_1 10
#define LOOPLIMIT_2 5

#define TIMERID_1 0
#define TIMERID_2 1

#define NUM_TIMER 2



void TimerCallback(TimerHandle_t pxTimer) {
    // printf("%d, ahihi, %d\n ", xTaskGetTickCount(), (int)pvTimerGetTimerID(pxTimer));
    static int loopCountArr[NUM_TIMER] = {};
    int timerID = (int)pvTimerGetTimerID(pxTimer);
    if (timerID == TIMERID_1) {
        printf("%d, %d, ahihi\n" ,xTaskGetTickCount(), loopCountArr[0]);
        loopCountArr[0]++;
        if (loopCountArr[0] == LOOPLIMIT_1) {
            if (xTimerStop(pxTimer, 0) == pdTRUE) {
                printf("Timer 1 stopped\n");
            }
            xTimerDelete(pxTimer, 10);
            
        } 
    } else if (timerID == TIMERID_2) {
        printf("%d, %d, ihaha\n",xTaskGetTickCount(), loopCountArr[1]);
        loopCountArr[1]++;
        if (loopCountArr[1] == LOOPLIMIT_2) {
            if (xTimerStop(pxTimer, 0) == pdTRUE) {
                printf("Timer 2 stopped\n");
            }
            xTimerDelete(pxTimer, 10);
        }       
    }
}

void app_main(void)
{
    TimerHandle_t timerHandle = xTimerCreate("timer1", pdMS_TO_TICKS(2000), pdTRUE, (void*)TIMERID_1, &TimerCallback);
    TimerHandle_t timerHandle2 = xTimerCreate("timer2", pdMS_TO_TICKS(3000), pdTRUE, (void *)TIMERID_2, &TimerCallback);
    if (timerHandle == NULL) {
        printf("Timer 1 not created!\n");
    } else {
        if (xTimerStart(timerHandle,0) == pdTRUE) {
            printf("Timer 1 started\n");
        }
    }
    if (timerHandle2 == NULL) {
        printf("Timer 2 not created!\n");
    } else {
        if (xTimerStart(timerHandle2,0) == pdTRUE) {
            printf("Timer 2 started\n");
        }
    }
}