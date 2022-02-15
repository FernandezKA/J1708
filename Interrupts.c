#include "main.h"
#include <stdlib.h>
#include "Interrupts.h"
// It's IRQ handler for UART_PC
void USART0_IRQHandler(void)
{
	// If receive data - push to the our FIFO buffer
	if (usart_flag_get(PC_UART, USART_FLAG_RBNE))
	{
		Push(&RxBuf, usart_data_receive(PC_UART));
		usart_flag_clear(PC_UART, USART_FLAG_RBNE);
	}
	// If we have unsended data into TX FIFO buf, send as TXBuff is empty
	else if (usart_flag_get(PC_UART, USART_FLAG_TBE))
	{
		if (GetSize(&TxBuf) != 0)
		{
			usart_data_transmit(PC_UART, Pull(&TxBuf));
		}
		else
		{
			usart_interrupt_disable(PC_UART, USART_INT_TBE);
			Clear(&TxBuf);
		}
	}
	return;
}
// It's IRQ handler for j1708 bus
void USART1_IRQHandler(void)
{ // Receive from j1708  bus
	if (usart_flag_get(J1708_UART, USART_FLAG_RBNE))
	{
		usart_flag_clear(J1708_UART, USART_FLAG_RBNE);
		TIMER_CNT(TIMER0) = 0;
		Push(&J1708_RxBuf, usart_data_receive(J1708_UART));
	}
	else if (usart_flag_get(J1708_UART, USART_FLAG_TBE))
	{ // Transmit to j1708 bus
		if(GetSize(&J1708_TxBuf) != 0){
			usart_data_transmit(J1708_UART, Pull(&J1708_TxBuf));
		}
		else{
			Clear(&J1708_TxBuf);
			usart_interrupt_disable(J1708_UART, USART_INT_TBE);
		}
	}
	else
	{ // Undefined behaviour
		
	}
	return;
}
// This IRQ handler for timing definition for j1708 bus
void TIMER0_UP_IRQHandler(void)
{
	timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
	TIMER_CNT(TIMER0) = 803; // Because times more than 10mS is new package, we parse overflow as new packet
}
// This IRQ handler for indicate activity and general timing definition
void TIMER1_IRQHandler(void)
{
	timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
	GPIO_OCTL(GPIOC) ^= (1 << 13);
}
