#ifndef _Interrupts_h_
#define _Interrupts_h_
#include "uart.h"
// For USART IRQ Handlers for communications
void USART0_IRQHandler(void);
void USART1_IRQHandler(void);
void TIMER0_UP_IRQHandler(void);
void TIMER1_IRQHandler(void);
void USBD_LP_CAN0_RX0_IRQHandler(void);
#endif
