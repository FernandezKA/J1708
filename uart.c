#include "uart.h"
#include "main.h"

void print(char *pMsg)
{
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
		usb_data_buffer[countSend - 1] = currChar;
	}
		if (USBD_CONFIGURED == usb_device_dev.status)
		{
		cdc_acm_data_send(&usb_device_dev, countSend);
		}
}
