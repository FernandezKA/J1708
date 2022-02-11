#include "main.h"
//#include "j1708.h"
//User global variables
typedef struct j1708 J1708;
//For communicate with PC
FIFO RxBuf;
FIFO TxBuf;
//For J1708 parsing
FIFO J1708_RxBuf;
FIFO J1708_TxBuf;
//It's variable for parsed packet
J1708 RxStruct;

//User init functions 
void SysInit(void);
//Main function
int main(){
	SysInit();
	nvic_irq_enable(USART0_IRQn, 3, 3);//For UART0_PC
	nvic_irq_enable(USART1_IRQn, 	1, 1);//For J1708 UART IRQ
	nvic_irq_enable(TIMER0_UP_IRQn, 4, 4);//For timmind definition
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
