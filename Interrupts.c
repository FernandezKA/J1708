#include "main.h"
#include <stdlib.h>
#include "Interrupts.h"

void USART0_IRQHandler(void){
	//If receive data - push to the our FIFO buffer
	if(usart_flag_get(USART0, USART_FLAG_RBNE)){
		UART_PC_RX(usart_data_receive(USART0), &RxBuf); 
		//usart_data_transmit(USART0, 0x64);
		usart_flag_clear(USART0, USART_FLAG_RBNE);
	}
	//If we have unsended data into TX FIFO buf, send as TXBuff is empty
	else if(usart_flag_get(USART0, USART_FLAG_TBE)){
		if(GetSize(&TxBuf) != 0){
			usart_data_transmit(USART0, Pull(&TxBuf));
		}
		else{
			usart_interrupt_disable(USART0, USART_INT_TBE);
		}
	}
	return;
}
