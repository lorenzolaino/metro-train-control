/*----------------------------------------------------------------------------
 * Name:    BlinkySingleLED_commanded_IE.c
 * Purpose: Turn on/off LED depending on input, managed via interrupt
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                       
#include <stdio.h>
#include "stm32f10x_it.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "controller.h" 
#include "RTX_Tasks.h"
#include "Serial.h"

/*----------------------------------------------------------------------------
  Configure Pin 0 (Emergency braking) with interrupt
 *----------------------------------------------------------------------------*/
void Configure_PB0_Emergency_Braking(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
   
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// Enable clock for GPIOB
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);     
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;	// PB0 is connected to EXTI_Line0
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;	// Enable interrupt
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	// Interrupt mode
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// Triggers on rising and falling edge
    EXTI_Init(&EXTI_InitStruct);		// Add to EXTI
 
    /* Add IRQ vector to NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;  // PB0 is connected to EXTI_Line0, which has EXTI0_IRQn vector
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	// Set priority
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;		// Set sub priority
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		// Enable interrupt
    NVIC_Init(&NVIC_InitStruct);				// Add to NVIC
}

/*----------------------------------------------------------------------------
  Configure Pin 1 (Stop signal) with interrupt
 *----------------------------------------------------------------------------*/
void Configure_PB1_Stop_Signal(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// Enable clock for GPIOB
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);     
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line1;	// PB1 is connected to EXTI_Line1
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;	// Enable interrupt
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	// Interrupt mode
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// Triggers on rising and falling edge
    EXTI_Init(&EXTI_InitStruct);		// Add to EXTI
 
    /* Add IRQ vector to NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;  // PB1 is connected to EXTI_Line1, which has EXTI1_IRQn vector
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	// Set priority
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;		// Set sub priority
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		// Enable interrupt
    NVIC_Init(&NVIC_InitStruct);				// Add to NVIC
}

/*----------------------------------------------------------------------------
  Configure Timer of 10ms
 *----------------------------------------------------------------------------*/
void Configure_Timer(void) {
	
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// enable timer clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

  /* Configure two bits for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* Enable the TIM2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // configure timer
  // read default values into TIM_TimeBaseStructure
  TIM_TimeBaseStructInit (& TIM_TimeBaseStructure);
  // PWM frequency = 1 KHz with 72MHz system clock
  // 72M/720 = 100000 Hz
  // 100000/1000 = 100 Hz
  TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /100000 - 1; // 719
  TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 999
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // Initialize TIM2 as specified in the TIM_TimeBaseInitStruct.
  TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);

  // PWM1 Mode configuration: Channel2
  // read default values into TIM_OCInitStructure
  TIM_OCStructInit (& TIM_OCInitStructure);
  // Set mode to PWM1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC2Init(TIM2 , &TIM_OCInitStructure);

  // Enable Timer
  TIM_Cmd(TIM2 , ENABLE);
	
	/* Enables the TIM2 Capture Compare channel 2 Interrupt source */
  TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE );
  /* Clear the TIM2 Capture Compare 2 flag */
  TIM_ClearFlag(TIM2, TIM_FLAG_CC2);
}

/*----------------------------------------------------------------------------
  Configure USART1 Interrupt
 *----------------------------------------------------------------------------*/
void USART1_NVIC_config(void) {
	
	NVIC_InitTypeDef NVIC_InitStructureUSART;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructureUSART.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructureUSART.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructureUSART.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructureUSART);
}

/*----------------------------------------------------------------------------
  Main Program
	CRH PC.11-10-9-8 define as Output - Braking
	CRL PC.2-1-0     define as Output - Acceleration
 *----------------------------------------------------------------------------*/
int main(void) {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH    = 0x00003333;               
	GPIOC->CRL    = 0x00000333;								
	GPIOB->CRH    = 0x88888888;
	GPIOB->CRL    = 0x88888888;
	
  Configure_PB0_Emergency_Braking();
	Configure_PB1_Stop_Signal();
	Configure_Timer(); 
	
	SER_Init_USART1();
	SER_Init_USART2();
	USART1_NVIC_config();
	
	initialize_Task();
	
}




