#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

//if change NO_OF_BUTTON, go to input_reading and change pinValue + buttonBuffer
#define NO_OF_BUTTON 		1
//wait time until debouncing
#define WAIT_TIME 			2
#define DURATION_1_SECOND 	100

#define BUTTON_PRESSED			0
#define BUTTON_RELEASED 		1

void read_button_task();
void init_button();

int getButtonValue(int num_of_button);

int getFlagButtonPress1s(int num_of_button);


#endif /* INC_INPUT_READING_H_ */