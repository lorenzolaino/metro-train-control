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
#include "swt.h"
#include "controller.h"

#define ms_Period 9 // 10 ms of period
#define swt_Id 0 // id of software timer 

volatile unsigned int SWT_STATE = 0;
int PROVA = 0; 
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

void Configure_PB0_Emergency_Braking(void) {
    /* Set variables used */
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
    
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;	// PB4 is connected to EXTI_Line4
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;	// Enable interrupt
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	// Interrupt mode
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	// Triggers on rising and falling edge
    EXTI_Init(&EXTI_InitStruct);		// Add to EXTI
 
    /* Add IRQ vector to NVIC */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;  // PB4 is connected to EXTI_Line4, which has EXTI4_IRQn vector
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	// Set priority
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;		// Set sub priority
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;		// Enable interrupt
    NVIC_Init(&NVIC_InitStruct);				// Add to NVIC
}

/*----------------------------------------------------------------------------
  Software timer initialization
 *----------------------------------------------------------------------------*/

void all_Init(void) {
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
  // 100000/100 = 1 KHz
  TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /100000 - 1; // 719
  TIM_TimeBaseStructure.TIM_Period = 100 - 1; // 99
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

void configure_Swt(void) {
	all_Init();
	swtInit();
	swtSetPeriod(swt_Id, ms_Period);
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/

int main(void) {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC->CRH    = 0x00003333;               /*PC.11-10-9-8 define as Output - Braking */
	GPIOC->CRL    = 0x00000333;								/*PC.2-1-0 define as Output - Acceleration */
	
  Configure_PB0_Emergency_Braking();
	configure_Swt(); 
	
	IN_INT=0;
	while(1){
			//wait ();
			//IN_INT=0;
		
			if (swtExpired(swt_Id)) {
				swtResetFlag(swt_Id);
				SWT_STATE = 1-SWT_STATE;
				
				check_Input();
				
			}
	}
}
