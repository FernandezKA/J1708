#include "main.h"
//User global variables
FIFO RxBuf;
FIFO TxBuf;
//User init functions 
void SysInit(void);
//Main function
int main(){
	SysInit();
	nvic_irq_enable(USART0_IRQn, 3, 3);//For UART0_PC 
	for(;;){
		//Get send our data
		if(GetSize(&TxBuf) != 0){
			usart_interrupt_enable(USART0, USART_INT_TBE);
		}
		//Get parse received data
		if(GetSize(&RxBuf) != 0){
				Push(&TxBuf, Pull(&RxBuf));
		}
	}
}
//This function combined all of init functions
void SysInit(void){
	//Init periphs
	GPIO_Init();
	USART0_Init();
	//Clear all of buffers
	Clear(&RxBuf);
	Clear(&TxBuf);
}
