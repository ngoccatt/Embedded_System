#include "button.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//button pin value
static int pinValue[NO_OF_BUTTON] = {14};

//buffer for button and a buffer before debouncing
static int buttonBufferBeforeDebouncing[NO_OF_BUTTON]= {BUTTON_RELEASED};
static int buttonBuffer[NO_OF_BUTTON] = {BUTTON_RELEASED};

//counter for button press and debouncing wait time
static int counterForButtonPress[NO_OF_BUTTON];
static int waitTimeTillDebouncing[NO_OF_BUTTON];

//some flags
static int flagButtonPress1s[NO_OF_BUTTON];

void init_button() {
    //init for each button
    for (int i = 0; i < NO_OF_BUTTON; i++) {
        gpio_reset_pin(pinValue[i]);
        gpio_set_direction(pinValue[i], GPIO_MODE_INPUT);
            // Configure GPIO pull-up/pull-down resistors.
        gpio_set_pull_mode(pinValue[i], GPIO_PULLUP_ONLY);

        //initialize previous buffer for each button
        buttonBufferBeforeDebouncing[i] = BUTTON_RELEASED;
        //initialize buffer for each button
        buttonBuffer[i] = BUTTON_RELEASED;
    }
}

void read_button_task(void *pvParameter) {
    while(true) {
        //read each button.
        for(int i = 0; i < NO_OF_BUTTON; i++) {
            //counting debounce. when waitTime = 0, check the level of button.
            if (waitTimeTillDebouncing[i] > 1) {
                waitTimeTillDebouncing[i]--;
            } else {
                //if current level of button is the same before debouncing, get the value
                if (buttonBufferBeforeDebouncing[i] == gpio_get_level(pinValue[i])) {
                    buttonBuffer[i] = buttonBufferBeforeDebouncing[i];
                } else { 
                    //if the current level of button is different from before debouncing, 
                    //assign a new value to the previous buffer and start counting debounce again
                    buttonBufferBeforeDebouncing[i] = gpio_get_level(pinValue[i]);
                    waitTimeTillDebouncing[i] = WAIT_TIME;
                }
                //button level get success. now process the button level.
                if (buttonBuffer[i] == BUTTON_PRESSED) {
                    if (counterForButtonPress[i] < DURATION_1_SECOND) {
                        counterForButtonPress[i]++;
                    } else {
                        flagButtonPress1s[i] = 1;
                    }
                } else {
                    counterForButtonPress[i] = 0;
                    flagButtonPress1s[i] = 0;
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
