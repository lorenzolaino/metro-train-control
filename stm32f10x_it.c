#include "stm32f10x_it.h"
#include "swt.h"

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

	if(GPIOB->IDR & in_pin) {
      GPIOC->ODR |= out_pin;                 // switch LED ON
	} else {
      GPIOC->ODR &= ~out_pin;                 // switch LED OFF
	}
}
