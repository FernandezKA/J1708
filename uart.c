#include "uart.h"
#include "main.h"

void print(char* pMsg){
	uint8_t countSend = 0;
	char lastChar = 0;
	char currChar = 0;
	bool isEnd = FALSE;
	while(!isEnd && countSend != 0xFF){
		lastChar = currChar;
		currChar = pMsg[countSend++];
		if((lastChar == 0x0A) && (currChar == 0x0D)){
			 isEnd = TRUE;
		}
		else if((lastChar == 0x0D) && (currChar == 0x0A)){
			 isEnd = TRUE;
		}
		//while((USART_STAT(PC_UART)&USART_STAT_TBE) != USART_STAT_TBE){__NOP();}
		//usart_data_transmit(PC_UART, currChar);
		usb_data_buffer[countSend] = currChar;
	}
	cdc_acm_data_send(&usb_device_dev, countSend);
}
