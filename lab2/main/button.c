#include "button.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//button pin value
static int pinValue[NO_OF_BUTTON] = {14};

//buffer for button and a buffer before debouncing
static int buttonBufferBeforeDebouncing[NO_OF_BUTTON]= {RELEASE};
static int buttonBuffer[NO_OF_BUTTON] = {RELEASE};

//counter for button press and debouncing wait time
static int counterForButtonPress[NO_OF_BUTTON];
static int waitTimeTillDebouncing[NO_OF_BUTTON];

//some flags
static int flagButtonPress1s[NO_OF_BUTTON];
static int flagButtonDelay[NO_OF_BUTTON];

void read_button_task(void *pvParameter) {
    while(true) {
        for(int i = 0; i < NO_OF_BUTTON; i++) {
            if (waitTimeTillDebouncing[i] > 1) {
                waitTimeTillDebouncing[i]--;
            } else {
                if (buttonBufferBeforeDebouncing[i] == gpio_get_level(pinValue[i])) {
                    buttonBuffer[i] = buttonBufferBeforeDebouncing[i];
                } else {
                    buttonBufferBeforeDebouncing[i] = gpio_get_level(pinValue[i]);
                    waitTimeTillDebouncing[i] = WAIT_TIME;
                }
                if (buttonBuffer[i] == PRESS) {
                    if (counterForButtonPress[i] < DURATION_1_SECOND) {
                        counterForButtonPress[i]++;
                    } else {
                        flagButtonPress1s[i] = 1;
                    }
                } else {
                    counterForButtonPress[i] = 0;
                    flagButtonPress1s[i] = 0;
                    flagButtonDelay[i] = 0;
                }
            }
        }
        vTaskDelay(10/portTICK_RATE_MS);
    }
}

int getButtonValue(int num_of_button) {
	if (num_of_button >= NO_OF_BUTTON) return -1;
	return buttonBuffer[num_of_button];
}

int getFlagButtonPress1s(int num_of_button) {
	if (num_of_button >= NO_OF_BUTTON) return -1;
	return flagButtonPress1s[num_of_button];
}

//this flag can only be pull down if button is released.
void setFlagButtonDelay(int num_of_button) {
	if (num_of_button >= NO_OF_BUTTON) return;
	flagButtonDelay[num_of_button] = 1;
}

int getFlagButtonDelay(int num_of_button) {
	if(num_of_button >= NO_OF_BUTTON) return -1;
	return flagButtonDelay[num_of_button];
}