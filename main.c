#include "main.h"
//#include "j1708.h"
#define PCKT_DELAY 803U
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
	nvic_irq_enable(USART0_IRQn, 2, 1);//For UART0_PC
	nvic_irq_enable(USART1_IRQn, 	1, 1);//For J1708 UART IRQ
	nvic_irq_enable(TIMER0_UP_IRQn, 2, 2);//For timming definition
	nvic_irq_enable(TIMER1_IRQn, 3, 3) ; //For led indicate activity
	//User main variables
	bool isParsed = FALSE;
	//Infinite loop
	for(;;){
		/*This part of code for send parsed J1708 packet*/
		//Get send our data
		if((GetSize(&TxBuf) != 0) && isParsed){
			usart_interrupt_enable(PC_UART, USART_INT_TBE);
		}
		else{
			 isParsed = FALSE;
		}
		/*This part of code for parse input j1708 packets*/
		//Get parse received data
		if(GetSize(&J1708_RxBuf) != 0 && TIMER_CNT(TIMER0) > PCKT_DELAY){
				 isParsed = GetPacket(&J1708_RxBuf, &RxStruct);
		}
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
	TIM1_Init();
	//Clear all of buffers
	Clear(&RxBuf);
	Clear(&TxBuf);
}
