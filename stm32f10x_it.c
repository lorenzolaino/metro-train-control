#include "stm32f10x_it.h"

extern int IN_INT;
void TIM2_IRQHandler(void) {
	unsigned int i = 1<<8;
	
  /* Clear TIM2 update interrupt */
  TIM_ClearFlag(TIM2, TIM_FLAG_CC2);

	// Toggle LED
  if(GPIOB->ODR & i)  GPIOB->ODR &= ~i;
  else GPIOB->ODR |= i;
}


void EXTI9_5_IRQHandler(void) {  // EXTI15_10_IRQn
	unsigned int in_pin=1<<8, out_pin = 1<<2;	
	EXTI_ClearFlag(EXTI_Line8);
  IN_INT=1;

	if(GPIOB->IDR & in_pin) {
      GPIOC->ODR |= out_pin;                 // switch LED ON
	} else {
      GPIOC->ODR &= ~out_pin;                 // switch LED OFF
	}
}
