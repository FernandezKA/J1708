#include "uart.h"
//This IRQ handler for UART_PC
void UART_PC_RX(const volatile uint32_t data, FIFO* buf){
	Push(buf, data);
	return;	
}
//This IRQ handler for j1708 bus
void UART_J1708_RX(const volatile uint32_t data, FIFO* buf, bool isEnd){
	if(!isEnd){//Time less or equal then interbyte space
		Push(buf, data);
	}
	else{//It's all of packet
		Push(buf, data);
		//TODO: Add parser calling
	}
	return;
}
