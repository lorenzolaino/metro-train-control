#include <RTL.h>
#include <stm32f10x.h>
#include <stdbool.h> 
#include "controller.h"
#include "stm32f10x_it.h"
#include "lever.h"

/*----------------------------------------------------------------------------
  Correctness constant and variable for handling max acceleration and no input
 *----------------------------------------------------------------------------*/
const int max_Acceleration_Timer = 400; // 4 seconds
const int max_No_Input_Timer = 300;			// 3 seconds
unsigned int max_Acceleration_Time = 0;
unsigned int max_No_Input_Time = 0;

/*----------------------------------------------------------------------------
  Current state of the lever 
 *----------------------------------------------------------------------------*/
int current_State = 1<<no_Acceleration_No_Braking;

extern OS_TID stop_Signal_Final_Task_ID;	

/*----------------------------------------------------------------------------
  Check Input: handle the input read from the position of the lever
 *----------------------------------------------------------------------------*/
void check_Input(void) {

	switch(GPIOB->IDR) {
		case 1<<str_Braking:
			manage_Input(str_Braking_Force);
			break;
		case 1<<med_Braking:
			manage_Input(med_Braking_Force);
			break;
		case 1<<min_Braking:
			manage_Input(min_Braking_Force);
			break;
		case 1<<no_Acceleration_No_Braking:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			set_Current_State(no_Acceleration_No_Braking);
			break;
		case 1<<min_Acceleration:
			manage_Input(min_Power);
			break;
		case 1<<med_Acceleration:
			manage_Input(med_Power);
			break;
		case 1<<max_Acceleration:
			max_Acceleration_Time++;
			if (max_Acceleration_Time >= max_Acceleration_Timer) {
				turn_Off_Led(1<<max_Power);
				turn_On_Led(1<<med_Power);
				set_Current_State(med_Power);
			} else {
				turn_Off_Led(1<<current_State);
				turn_On_Led(1<<max_Power);
				set_Current_State(max_Power);
			}
			break;
		case 0x00000000: //Possibilità di mettere 0 
			max_No_Input_Time++;
			if (max_No_Input_Time >= max_No_Input_Timer) {
				os_evt_set(0x01, stop_Signal_Final_Task_ID);
			}
			break;
	}
}

/*----------------------------------------------------------------------------
  Manage Input: turn off the current state led, turn on the active led 
								and reset the max acceleration timer
 *----------------------------------------------------------------------------*/
void manage_Input(int pin) {
	max_Acceleration_Time = 0;
	turn_Off_Led(1<<current_State);
	turn_On_Led(1<<pin);
	set_Current_State(pin);
}

/*----------------------------------------------------------------------------
  Turn On Led: turn on the led on the GPIOC 
 *----------------------------------------------------------------------------*/
void turn_On_Led(int out_pin) {
	GPIOC->ODR |= out_pin;
}

/*----------------------------------------------------------------------------
  Turn Off Led: turn off the led on the GPIOC 
 *----------------------------------------------------------------------------*/
void turn_Off_Led(int out_pin) {
	GPIOC->ODR &= ~out_pin;
}

/*----------------------------------------------------------------------------
  Set Current State: set the current state of the lever  
 *----------------------------------------------------------------------------*/
void set_Current_State(int state) {
	current_State = state;
}

/*----------------------------------------------------------------------------
  Get Current State: get the current state of the lever  
 *----------------------------------------------------------------------------*/
int get_Current_State(void) {
	return current_State;
}
