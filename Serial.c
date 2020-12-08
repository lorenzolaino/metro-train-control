/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *            __DBG_ITM   - ITM SWO interface
 *                        - USART1 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                       /* STM32F10x definitions         */

#ifdef __DBG_ITM
volatile int ITM_RxBuffer = ITM_RXBUFFER_EMPTY;  /*  CMSIS Debug Input        */
#endif


/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Init (void) {
#ifndef __DBG_ITM
  int i;

  RCC->APB1ENR |=  (   1UL <<  0);         /* enable clock Alternate Function */
  AFIO->MAPR   &= ~(   1UL <<  3);         /* clear USART1 remap              */

  RCC->APB1ENR |=  (   1UL <<  2);         /* enable GPIOA clock              */
  GPIOA->CRL   &= ~(0xFFUL <<  8);         /* clear PA9, PA10                 */
  GPIOA->CRL   |=  (0x0BUL <<  8);         /* USART1 Tx (PA9) output push-pull*/ //PA2
  GPIOA->CRL   |=  (0x04UL <<  12);         /* USART1 Rx (PA10) input floating */ //PA3

  RCC->APB1ENR |=  (   1UL << 17);         /* enable USART1 clock             */

  /* 115200 baud, 8 data bits, 1 stop bit, no flow control */
  USART2->CR1   = 0x002C;                  /* enable RX, TX                   */
  USART2->CR2   = 0x0000;
  USART2->CR3   = 0x0000;                  /* no flow control                 */
  USART2->BRR   = 0x0271;
  for (i = 0; i < 0x1000; i++) __NOP();    /* avoid unwanted output           */
  USART2->CR1  |= 0x2000;                  /* enable USART                   */

#endif
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int c) {

#ifdef __DBG_ITM
    ITM_SendChar(c);
#else
  while (!(USART2->SR & USART_SR_TXE));
  USART2->DR = (c & 0x1FF);
#endif
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {

#ifdef __DBG_ITM
  while (ITM_CheckChar() != 1) __NOP();
  return (ITM_ReceiveChar());
#else
  while (!(USART2->SR & USART_SR_RXNE));
  return (USART2->DR & 0xFF);
#endif
}


/*----------------------------------------------------------------------------
  Check if a character is received
 *----------------------------------------------------------------------------*/
int SER_CheckChar (void) {

#ifdef __DBG_ITM
  return (ITM_CheckChar());
#else
  if (USART2->SR & USART_SR_RXNE)
    return (1);
  else
    return (0);
#endif
}