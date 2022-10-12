#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "button.h"

int led = 2;

int toggle = 0;

#define PRESSED 0
#define RELEASE 1
int button_state = RELEASE;

TaskHandle_t xHandle;

void print_student_id_task(void *pvParameter) {


    while (true) 
    {
        printf("1913774 - 1914213 - 1912750 - 1915677\n");
        if (toggle == 0) {
            gpio_set_level(led, 0);
        } else {
            gpio_set_level(led, 1);
        }
        toggle = 1 - toggle;
        // printf("%d",gpio_get_level(0));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void print_esp32_task_state_machine(void *pvParameter) {

    while(true) {
       switch (button_state) {
        case RELEASE:
            if (getButtonValue(0) == 0) {
                button_state = PRESSED;
                printf("ESP32\n");
            }
            break;
        case PRESSED:
            if (getButtonValue(0) == 1) {
                button_state = RELEASE;
            }
            break;
       }
       vTaskDelay(10/ portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}


void gpio_init() {
    gpio_set_direction(led, GPIO_MODE_OUTPUT);
}


void app_main(void)
{
    
    init_button();
    gpio_init();

    xTaskCreate(&print_student_id_task, "sync", 2048, NULL, 5, NULL);
    xTaskCreate(&print_esp32_task_state_machine, "async", 2048, NULL, 10, NULL);
    xTaskCreate(&read_button_task, "readbtn", 2048, NULL, 15, NULL);

}