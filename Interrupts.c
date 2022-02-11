#include "main.h"
#include <stdlib.h>
#include "Interrupts.h"
//It's IRQ handler for UART_PC
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
			//Undefined behaviour
			//usart_interrupt_disable(USART0, USART_INT_TBE);
		}
	}
	return;
}
//It's IRQ handler for j1708 bus
void USART1_IRQHandler(void){
	if(usart_flag_get(USART1, USART_FLAG_RBNE)){
		usart_flag_clear(USART1, USART_FLAG_RBNE);
	}
	else if(usart_flag_get(USART1, USART_FLAG_TBE)){
		
	}
	else{
		
	}
	return;
}
//This IRQ handler for indicate activity
void TIMER0_UP_IRQHandler(void){
	timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_UP);
	if(TIMER_CNT(TIMER0) > 8){//It's next J1708 packet
		
	}
	else{
		//GetPacket(&J1708_RxBuf, );//
	}
	GPIO_OCTL(GPIOC)^= (1<<13);
}
