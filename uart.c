#include "uart.h"

void UART_PC_RX(const volatile uint32_t data, FIFO* buf){
	Push(buf, data);
	return;	
}

