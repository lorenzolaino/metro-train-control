

#ifndef __STM32F10x_IT_H  // Define to prevent recursive inclusion
#define __STM32F10x_IT_H

#include "stm32f10x.h"
void TIM2_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void set_Stop_Signal(int is_Active);
#endif


