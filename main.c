#include "main.h"
//User global variables
//For communicate with PC
FIFO RxBuf;
FIFO TxBuf;
//For J1708 parsing
FIFO J1708_RxBuf;
FIFO J1708_TxBuf;
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
		
		//for(uint32_t i = 0; i < 0xFFFFFF; ++i){__NOP();}
	}
}
//This function combined all of init functions
void SysInit(void){
	//Init periphs
	CLK_Init();
	GPIO_Init();
	USART0_Init();
	USART1_Init();
	TIM0_Init();
	//Clear all of buffers
	Clear(&RxBuf);
	Clear(&TxBuf);
}
