#include "stm32f10x_it.h"
#include "swt.h"
#include "controller.h"

extern int IN_INT;

void TIM2_IRQHandler(void) {
  /* Clear TIM2 update interrupt */
  TIM_ClearFlag(TIM2, TIM_FLAG_CC2);
//  ticks++;
	swtUpdate();
}

void EXTI0_IRQHandler(void) {  // EXTI0_IRQn
	unsigned int in_pin=1<<0, out_pin = 1<<11;	
	EXTI_ClearFlag(EXTI_Line0);
  IN_INT=1;
	
	turn_Off_Led(1<<get_Current_State());

	if(GPIOB->IDR & in_pin) {
		GPIOC->ODR |= out_pin;		// switch LED ON
		set_Emergency_Breaking_Signal(1);
	} 
}

void EXTI1_IRQHandler(void) {  // EXTI1_IRQn
	unsigned int in_pin=1<<1, out_pin = 1<<10;	
	EXTI_ClearFlag(EXTI_Line1);
  IN_INT=1;
	
	turn_Off_Led(1<<get_Current_State());

	if(GPIOB->IDR & in_pin) {
		GPIOC->ODR |= out_pin;		// switch LED ON
		set_Stop_Signal(1);
	} else {
		GPIOC->ODR &= ~out_pin;   // switch LED OFF
		
		while(GPIOB->IDR != 1<<5) {
		
		}
		set_Stop_Signal(0);
	}
}
