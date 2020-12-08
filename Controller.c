#include <RTL.h>
#include <stm32f10x.h>
#include <stdbool.h> 
#include "controller.h"
#include "swt.h"
#include "stm32f10x_it.h"

// Output on GPIO/C
#define max_Power 2
#define med_Power 1
#define min_Power 0
#define max_Braking_Force 11
#define str_Braking_Force 10
#define med_Braking_Force 9
#define min_Braking_Force 8

// Input on GPIO/B
#define max_Acceleration 8
#define med_Acceleration 7
#define min_Acceleration 6
#define no_Acceleration_No_Braking 5
#define min_Braking 4
#define med_Braking 3
#define str_Braking 2

// Correctness constant
int max_Acceleration_Timer = 400; // 4 seconds
int max_No_Input_Timer = 300;			// 3 seconds

int current_State = 1<<no_Acceleration_No_Braking;

extern OS_TID stop_Signal_Final_Task_ID;

void initial_Configuration(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH    = 0x00003333;               /*PC.8-9-10-11 define as Output - Braking */
	GPIOC->CRL    = 0x00000333;								/*PC.0-1-2 define as Output - Acceleration */
}	

unsigned int max_Acceleration_Time = 0;
unsigned int max_No_Input_Time = 0;

void check_Input(void) {

	switch(GPIOB->IDR) {
		case 1<<str_Braking:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<str_Braking_Force);
			set_Current_State(str_Braking_Force);
			break;
		case 1<<med_Braking:
			max_Acceleration_Time = 0; 
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<med_Braking_Force);
			set_Current_State(med_Braking_Force);
			break;
		case 1<<min_Braking:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<min_Braking_Force);
			set_Current_State(min_Braking_Force);
			break;
		case 1<<no_Acceleration_No_Braking:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			set_Current_State(no_Acceleration_No_Braking);
			break;
		case 1<<min_Acceleration:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<min_Power);
			set_Current_State(min_Power);
			break;
		case 1<<med_Acceleration:
			max_Acceleration_Time = 0;
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<med_Power);
			set_Current_State(med_Power);
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

void turn_On_Led(int out_pin) {
	GPIOC->ODR |= out_pin;
}

void turn_Off_Led(int out_pin) {
	GPIOC->ODR &= ~out_pin;
}

void set_Current_State(int state) {
	current_State = state;
}

int get_Current_State(void) {
	return current_State;
}
