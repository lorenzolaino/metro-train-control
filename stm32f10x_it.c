#include <rtl.h>
#include <stdio.h>
#include "stm32f10x_it.h"
#include "controller.h"
#include "RTX_Tasks.h"
#include "Serial.h"


extern OS_TID background_Task_ID , emergency_Task_ID, stop_Signal_Task_ID, traffic_Messages_Task_ID;

int is_Emergency_Brake_Active = 0; 
int is_Stop_Signal_Active = 0;
int cmd; 

/*----------------------------------------------------------------------------
  Interrupt Handler for the 10ms timer: it wakes up the background task
 *----------------------------------------------------------------------------*/
void TIM2_IRQHandler(void) {
  /* Clear TIM2 update interrupt */
  TIM_ClearFlag(TIM2, TIM_FLAG_CC2);

	if (is_Emergency_Brake_Active || is_Stop_Signal_Active) {
		return;
	} 
	
	isr_evt_set(0x01, background_Task_ID);
}

/*----------------------------------------------------------------------------
  Interrupt Handler for pin 0: it wakes up the emergency brake task
 *----------------------------------------------------------------------------*/
void EXTI0_IRQHandler(void) {  // EXTI0_IRQn
	EXTI_ClearFlag(EXTI_Line0);
	is_Emergency_Brake_Active = 1; 
	isr_evt_set(0x01, emergency_Task_ID);
}

/*----------------------------------------------------------------------------
  Interrupt Handler for pin 1: it wakes up the stop signal task
 *----------------------------------------------------------------------------*/
void EXTI1_IRQHandler(void) {  // EXTI1_IRQn
	EXTI_ClearFlag(EXTI_Line1);
	isr_evt_set(0x01, stop_Signal_Task_ID);
}

/*----------------------------------------------------------------------------
  Interrupt Handler for USART1: it wakes up the traffic messages task
 *----------------------------------------------------------------------------*/
void USART1_IRQHandler(void) {
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		cmd = USART_ReceiveData(USART1);
		isr_evt_set(0x01, traffic_Messages_Task_ID);
	}
}

/*----------------------------------------------------------------------------
  Set Stop Singal: set the stop signal active flag
 *----------------------------------------------------------------------------*/
void set_Stop_Signal(int is_Active) {
	is_Stop_Signal_Active = is_Active; 
}



