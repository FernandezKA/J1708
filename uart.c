#include "uart.h"
//This IRQ handler for UART_PC
void UART_PC_RX(const volatile uint32_t data, FIFO* buf){
	Push(buf, data);
	return;	
}
//This IRQ handler for j1708 bus
void J1708_Rx(const volatile uint32_t data, FIFO* buf){
	
}
