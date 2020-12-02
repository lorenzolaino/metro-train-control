#include "stm32f10x_it.h"
#include "swt.h"
#include "controller.h"
#include "RTX_Tasks.h"

extern int IN_INT;
extern OS_TID background_Task_ID , emergency_Task_ID, stop_Signal_Task_ID;

int is_Emergency_Brake_Active = 0; 
int is_Stop_Signal_Active = 0;

void TIM2_IRQHandler(void) {
  /* Clear TIM2 update interrupt */
  TIM_ClearFlag(TIM2, TIM_FLAG_CC2);

	if (is_Emergency_Brake_Active || is_Stop_Signal_Active) {
		return;
	} 
	
	isr_evt_set(0x01, background_Task_ID);
}

void EXTI0_IRQHandler(void) {  // EXTI0_IRQn
	EXTI_ClearFlag(EXTI_Line0);
  IN_INT=1;
	is_Emergency_Brake_Active = 1; 
	isr_evt_set(0x01, emergency_Task_ID);
}

void EXTI1_IRQHandler(void) {  // EXTI1_IRQn
	EXTI_ClearFlag(EXTI_Line1);
  IN_INT=1;
	isr_evt_set(0x01, stop_Signal_Task_ID);
}

void set_Stop_Signal(int is_Active) {
	is_Stop_Signal_Active = is_Active; 
}

