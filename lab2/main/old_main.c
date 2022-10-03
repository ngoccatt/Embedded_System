#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

int period = 1000;

int button_num = 14; // GPIO0
int button_en = 3; //GPIO 3 = EN
int led = 2;

int toggle = 0;

TaskHandle_t xHandle;

void print_student_id_task(void *pvParameter) {


    while (true) 
    {
        printf("1912750\n");
        if (toggle == 0) {
            gpio_set_level(led, 0);
        } else {
            gpio_set_level(led, 1);
        }
        toggle = 1 - toggle;
        vTaskDelay(period / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void print_esp32_task(void *pvParameter) {

    while(true) {
        vTaskSuspend(NULL);
        printf("ESP32\n");
        // vTaskDelay(period / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void ISR_task(void *pvParameter) {

    // Resume the suspended task.
    xTaskResumeFromISR(xHandle);
     // We should switch context so the ISR returns to a different task.
     // NOTE:  How this is done depends on the port you are using.  Check
     // the documentation and examples for your port.
    portYIELD_FROM_ISR();

}

void app_main(void)
{
   gpio_reset_pin(button_num);
   gpio_set_direction(button_num, GPIO_MODE_INPUT);
    // 2 dong nay la can thiet de gpio hoat dong!! :o :o :o why, wtf??
    // Configure GPIO pull-up/pull-down resistors.
   gpio_set_pull_mode(button_num, GPIO_PULLUP_ONLY);
   // GPIO set interrupt trigger type.
   gpio_set_intr_type(button_num, GPIO_INTR_NEGEDGE);

   gpio_set_direction(led, GPIO_MODE_OUTPUT);


   xTaskCreate(&print_student_id_task, "sync", 2048, NULL, 5, NULL);
   xTaskCreate(&print_esp32_task, "async", 2048, NULL, 10, &xHandle);
   gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
   gpio_isr_handler_add(button_num, &ISR_task, NULL);

   
   printf("a\n"); 

}