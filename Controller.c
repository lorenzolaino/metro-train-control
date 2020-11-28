#include <stm32f10x.h>
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

int current_State = no_Acceleration_No_Braking;
int stop_Signal_Active = 0; 
int emergency_Breaking_Active = 0; 

void initial_Configuration(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH    = 0x00003333;               /*PC.8-9-10-11 define as Output - Braking */
	GPIOC->CRL    = 0x00000333;								/*PC.0-1-2 define as Output - Acceleration */
}	

void check_Input(void) {
	if (stop_Signal_Active) {
		return; 
	}
	
	if (emergency_Breaking_Active) {
		return; 
	}
	
	switch_Led(1<<max_Acceleration, 1<<max_Power);
	switch_Led(1<<med_Acceleration, 1<<med_Power);
	switch_Led(1<<min_Acceleration, 1<<min_Power);
	
	switch_Led(1<<min_Braking, 1<<min_Braking_Force);
	switch_Led(1<<med_Braking, 1<<med_Braking_Force);
	switch_Led(1<<str_Braking, 1<<str_Braking_Force);
}

void switch_Led(int in_pin, int out_pin) {
	if(GPIOB->IDR & in_pin) {
		GPIOC->ODR |= out_pin;	// switch LED ON
	} else {
		GPIOC->ODR &= ~out_pin;	// switch LED OFF
	}
}

void set_Stop_Signal(int is_Active) {
	stop_Signal_Active = is_Active; 
}

void set_Emergency_Breaking_Signal(int is_Active) {
	emergency_Breaking_Active = is_Active; 
}

void set_Current_State(int state) {
	current_State = state; 
}

void switch_Off_Current_State(void) {
	GPIOC->ODR &= ~1<<current_State; //switch current state off
}
