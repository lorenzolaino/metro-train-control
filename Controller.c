#include <stm32f10x.h>
#include <stdbool.h> 
#include "controller.h"
#include "swt.h"

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

bool stop_Signal_Active = 0; 
bool emergency_Breaking_Active = 0; 

int current_State = 1<<no_Acceleration_No_Braking;

void initial_Configuration(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH    = 0x00003333;               /*PC.8-9-10-11 define as Output - Braking */
	GPIOC->CRL    = 0x00000333;								/*PC.0-1-2 define as Output - Acceleration */
}	

void check_Input(void) {
	
	if(emergency_Breaking_Active) {
		return;
	}
	
	if(stop_Signal_Active) {
		return; 
	}
	 
	switch(GPIOB->IDR) {
		case 1<<str_Braking:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<str_Braking_Force);
			set_Current_State(str_Braking_Force);
			break;
		case 1<<med_Braking:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<med_Braking_Force);
			set_Current_State(med_Braking_Force);
			break;
		case 1<<min_Braking:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<min_Braking_Force);
			set_Current_State(min_Braking_Force);
			break;
		case 1<<no_Acceleration_No_Braking:
			turn_Off_Led(1<<current_State);
			set_Current_State(no_Acceleration_No_Braking);
			break;
		case 1<<min_Acceleration:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<min_Power);
			set_Current_State(min_Power);
			break;
		case 1<<med_Acceleration:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<med_Power);
			set_Current_State(med_Power);
			break;
		case 1<<max_Acceleration:
			turn_Off_Led(1<<current_State);
			turn_On_Led(1<<max_Power);
			set_Current_State(max_Power);
			break;
		
	}
}

void turn_On_Led(int out_pin) {
	GPIOC->ODR |= out_pin;
}

void turn_Off_Led(int out_pin) {
	GPIOC->ODR &= ~out_pin;
}

void set_Stop_Signal(bool is_Active) {
	stop_Signal_Active = is_Active; 
}

void set_Emergency_Breaking_Signal(bool is_Active) {
	emergency_Breaking_Active = is_Active; 
}

void set_Current_State(int state) {
	current_State = state;
}

int get_Current_State(void) {
	return current_State;
}
