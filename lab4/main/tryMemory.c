#include <stdio.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include <time.h>
#include <string.h>

struct QueueData {
    int requestID;
    char message[2048];
};

QueueHandle_t xQueue;

void xQueue_manager(void * pvParameter) {
    // create a queue, each slot contant a pointer, because arcording to the website,
    // this struct is "too big!"
    int first = true;
    for (; ;) {
        if (first) {
            xQueue = xQueueCreate(10, sizeof(struct QueueData *));
            if (xQueue == NULL) {
                printf("Failed to create queue: not enough RAM.");
            }
            first = false;
        }
        struct QueueData *pData = malloc(sizeof(struct QueueData));
        if (pData == NULL) {
            printf("Dead");
            for(int i = 0; i < 1000000; i++) {
                //if this task is stop, that mean we can't no longer malloc memory, that's why pData = NULL.
                // If I don't check this condition and break it, this will cause Fatal Error: Store Prohibited, because
                // since pData is NULL, access to pData->requestID is not valid (access NULL pointer) and crash the program.
            }
            break;
        }
        pData->requestID = 1;
        strcpy(pData->message, "Task1");
        //&pData -> lay dia chi cua con tro pData, o nho tai dia chi nay dang chua gia tri tro? toi struct QueueData
        if (xQueueSend(xQueue, (void *)&pData , (TickType_t)10) == errQUEUE_FULL) {
            //this is used to see the effect happen when queue is full.
            //and about why it run pretty slow, even, vTaskDelay is just 10 tick... honestly, I don't know :'(
            printf("Queue overload!");
        }
        vTaskDelay(10);
    }
    vTaskDelete(NULL);
}








void app_main(void)
{
    xTaskCreate(&xQueue_manager, "queue_manage", 2048, NULL, 10, NULL);
}