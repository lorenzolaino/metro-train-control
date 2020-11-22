/*----------------------------------------------------------------------------
 * Name:    BlinkySingleLED_commanded_IE.c
 * Purpose: Turn on/off LED depending on input, managed via interrupt
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                       /* STM32F103 definitions         */
#include "stm32f10x_it.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
int WAIT;
int IN_INT=0;

/*----------------------------------------------------------------------------
  wait function
 *----------------------------------------------------------------------------*/
void wait (void)  {
  int  d, j;
  WAIT = 1;
  for (j=0; j<5; j++)
		for (d = 0; d < 1000; d++);
  WAIT = 0;
}

void Configure_PB8(void) {
    /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// Enable clock for GPIOB
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);     
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line8;	// PB15 is connected to EXTI_Line15
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;	// Enable interrupt
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	// Interrupt mode
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// Triggers on rising and falling edge
    EXTI_Init(&EXTI_InitStruct);		// Add to EXTI
 
    /* Add IRQ vector to NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;  // PB15 is connected to EXTI_Line15, which has EXTI15_10_IRQn vector
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	// Set priority
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;		// Set sub priority
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		// Enable interrupt
    NVIC_Init(&NVIC_InitStruct);				// Add to NVIC
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main(void) {
	
 RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRL    = 0x00000300;               /*PC.2 define as Output*/

  Configure_PB8();
	
	IN_INT=0;
	while(1){
			wait ();
			IN_INT=0;
	}
}
