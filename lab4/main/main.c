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
    int dataID;
    char message[20];
    char reject;
};

QueueHandle_t xQueue;

void xQueue_manager(void * pvParameter) {
    // create a queue, each slot contant a pointer, because arcording to the website,
    // this struct is "too big!"

    xQueue = xQueueCreate(10, sizeof(struct QueueData *));
    if (xQueue == NULL) {
        printf("Failed to create queue: not enough RAM.");
    }
    vTaskDelete(NULL);     
}

void recepiton_Task(void * pvParameter) {
    time_t t;

    srand((unsigned) time(&t));
    for (; ;) {
        while (xQueue == NULL) {
             // do nothing
        }
        int ranJob = (rand() % 4);
        int ranDelay = (rand() % 5) + 1;
        // allocate a dynamic space for the data, since our queue only save the "copy" of the pointer.
        struct QueueData *xData = malloc(sizeof(struct QueueData));

        if (xData != NULL) {
            switch(ranJob) {
                case 0:
                    xData->dataID = 0;
                    strcpy(xData->message, "LED");
                    xData->reject = 0;
                break;
                case 1:
                    xData->dataID = 1;
                    strcpy(xData->message, "PUMP");
                    xData->reject = 0;
                break;
                case 2:
                    xData->dataID = 2;
                    strcpy(xData->message, "FAN");
                    xData->reject = 0;
                break;
                case 3:
                    xData->dataID = 99;
                    strcpy(xData->message, "noob");
                    xData->reject = 0;
                break;
            }
            // notice here: we pass the *address of the pointer* (&xData) to the function
            // just understand it simply like this:
            // xQueueSend require a "pointer type" for its pvItemToQueue, A pointer to the item that is to be placed on the queue, 
            // our xQueue queue keep a copy of items, which data type is (QueueData *)
            // so we have to pass the "pointer to our item" to the function, since our item is (QueueData *), 
            // then we have to pass (QueueData **)
            // use & to create (QueueData *) -> (QueueData **).
            // we then can use memcpy() to copy src -> dest. 
            // memcpy(pointer to dest (our queue - has items as type (QueueData *)), pointer to src (&xData), size of data);
            if (xQueueSend(xQueue, (void *)&xData, 100) == errQUEUE_FULL) {
                printf("Failed to Import job with ID %d", ranJob);
            }
        } else {
            // if malloc is not successful, it will return NULL.
            printf("Can't allocate new struct");
        }
        vTaskDelay(pdMS_TO_TICKS(100 * ranDelay));
    }
    vTaskDelete(NULL);
}

void led_Task(void* pvParameter) {
    for (; ;) {
        struct QueueData *pRxMessage;
        if (xQueue != NULL) {
            if (xQueueReceive(xQueue, &pRxMessage, (TickType_t)10) == pdPASS) {
                if (pRxMessage->dataID == 0) {
                    printf("%s\n", pRxMessage->message);
                    // always remember to free the memory when done.
                    free(pRxMessage);
                } else {
                    printf("led: received %s, but it's not my task\n", pRxMessage->message);
                    if (pRxMessage->reject < 3) {
                        pRxMessage->reject++;
                        xQueueSendToFront(xQueue, (void *)&pRxMessage, (TickType_t)10);
                    } else {
                        printf("This task %s is rejected %d times, skiping the task\n", pRxMessage->message, pRxMessage->reject);
                        free(pRxMessage);
                    }
                }
            } else {
                // printf("led: queue empty\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

void pump_Task(void* pvParameter) {
    for (; ;) {
        struct QueueData *pRxMessage;
        if (xQueue != NULL) {
            if (xQueueReceive(xQueue, &pRxMessage, (TickType_t)10) == pdPASS) {
                if (pRxMessage->dataID == 1) {
                    printf("%s\n", pRxMessage->message);
                    // always remember to free the memory when done.
                    free(pRxMessage);
                } else {
                    printf("pump: received %s, but it's not my task\n", pRxMessage->message);
                    if (pRxMessage->reject < 3) {
                        pRxMessage->reject++;
                        xQueueSendToFront(xQueue, (void *)&pRxMessage, (TickType_t)10);
                    } else {
                        printf("This task %s is rejected %d times, skiping the task\n", pRxMessage->message, pRxMessage->reject);
                        free(pRxMessage);
                    }
                }
            } else {
                // printf("pump: queue empty\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}

void fan_Task(void* pvParameter) {
    for (; ;) {
        struct QueueData *pRxMessage;
        if (xQueue != NULL) {
            if (xQueueReceive(xQueue, &pRxMessage, (TickType_t)10) == pdPASS) {
                if (pRxMessage->dataID == 2) {
                    printf("%s\n", pRxMessage->message);
                    // always remember to free the memory when done.
                    free(pRxMessage);
                } else {
                    printf("fan: received %s, but it's not my task\n", pRxMessage->message);
                    if (pRxMessage->reject < 3) {
                        pRxMessage->reject++;
                        xQueueSendToFront(xQueue, (void *)&pRxMessage, (TickType_t)10);
                    } else {
                        printf("This task %s is rejected %d times, skiping the task\n", pRxMessage->message, pRxMessage->reject);
                        free(pRxMessage);
                    }
                }
            } else {
                // printf("fan: queue empty\n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}



void app_main(void)
{
    xTaskCreate(&xQueue_manager, "queue_manage", 2048, NULL, 10, NULL);
    xTaskCreate(&recepiton_Task, "rec", 2048, NULL, 10, NULL);
    xTaskCreate(&led_Task, "led", 2048, NULL, 10, NULL);
    xTaskCreate(&pump_Task, "pump", 2048, NULL, 10, NULL);
    xTaskCreate(&fan_Task, "fan", 2048, NULL, 10, NULL);
}