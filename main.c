#include "main.h"
// User defines
#define PCKT_DELAY 803U
// User global variables
// For communicate with PC
FIFO RxBuf;
FIFO TxBuf;
// For J1708 parsing
FIFO J1708_RxBuf;
FIFO J1708_TxBuf;
// It's variable for parsed packet
J1708 RxStruct;
J1708 TxStruct;
// FSM for input packets
enum RS232_FSM RecievedPacket = getPriority;
// User static functions definition
static inline void SysInit(void);
/*********************************************************************/
int main()
{
	SysInit();
	nvic_irq_enable(USART0_IRQn, 2, 1);	   // For UART0_PC
	nvic_irq_enable(USART1_IRQn, 1, 1);	   // For J1708 UART IRQ
	nvic_irq_enable(TIMER0_UP_IRQn, 2, 2); // For timming definition
	nvic_irq_enable(TIMER1_IRQn, 3, 3);	   // For led indicate activity
	// User main variables
	static bool isParsed = FALSE;
	static bool is_J1708_Completed = FALSE;
	static uint16_t countDataParsed = 0;
	print("J1708 adapter \n\r");
	print("2022-02-15 ver. 0.1\n\r");
	// Infinite loop
	for (;;)
	{
		/********************************************************************/
		// This part of code for J1708 -> RS232 parsing
		/********************************************************************/
		/*This part of code for send parsed J1708 packet*/
		// Get send our data
		if ((GetSize(&TxBuf) != 0) && isParsed)
		{
			usart_interrupt_enable(PC_UART, USART_INT_TBE);
		}
		else
		{
			isParsed = FALSE;
		}
		/*This part of code for parse input j1708 packets*/
		// Get parse received data
		if (GetSize(&J1708_RxBuf) != 0 && TIMER_CNT(TIMER0) > PCKT_DELAY)
		{
			isParsed = GetPacket(&J1708_RxBuf, &RxStruct);
			Clear(&J1708_RxBuf);
		}
		/********************************************************************/
		// This part of code for RS232 -> J1708 parsing
		/********************************************************************/
		/*Input packet struct:
		 *Priority  (1 - 8)
		 *Size      (Max buff size 256U)
		 *MID       (0-68)
		 *Data
		 */
		/*****************************************************************/
		// Get check for packet complete
		if (RecievedPacket == Complete)
		{
			// Check priority timings
			if (TIMER_CNT(TIMER0) > PCKT_DELAY + TxStruct.Priority * 10)
			{
				// Get send packet
				SendPacket(&TxStruct, &J1708_TxBuf);
				is_J1708_Completed = TRUE;
			}
		}
		// When all of data is sended, reset flag
		if (GetSize(&J1708_TxBuf) != 0 && is_J1708_Completed)
		{
			//TODO Add push to FIFO Buf PC
			usart_interrupt_enable(J1708_UART, USART_INT_TBE);
			RecievedPacket = getPriority;
			is_J1708_Completed = FALSE;
			TIMER_CNT(TIMER0) = 0;
		}
		
		
		
		//Get parse Rx UART_PC
		if(GetSize(&RxBuf)!= 0){
			 switch(RecievedPacket){
				 case getPriority:
					TxStruct.Priority =(uint16_t) Pull(&RxBuf);
					if(TxStruct.Priority < 1 || TxStruct.Priority > 8){
						print("Invalid priority. Buffer clean \n\r");
						Clear(&RxBuf);
						RecievedPacket = getPriority; 
					}
					else{
					RecievedPacket = getSize;
					}
				 break;
				 
				 case getSize: 
					TxStruct.Size =(uint8_t) Pull(&RxBuf);
					if(TxStruct.Size > 64){
						print("Invalid size \n\r");
						Clear(&RxBuf);
						RecievedPacket = getPriority;
					}
					else{
						RecievedPacket = getMID;				 
					}
				 break;
				 
				 case getMID:
					TxStruct.MID = (uint8_t) Pull(&RxBuf);
					if(TxStruct.MID > 68){
						print("Invalid MID. Buffer clean \n\r");
						Clear(&RxBuf);
						RecievedPacket = getPriority;
					}
					else{
						RecievedPacket = getData;
					}
				 break;
				 
				 case getData:
					 if (countDataParsed < TxStruct.Size - 1){
						TxStruct.Data[countDataParsed++] =(uint8_t) Pull(&RxBuf); 
					 }
					 else{
						TxStruct.Data[countDataParsed] =(uint8_t) Pull(&RxBuf);
						countDataParsed = 0;
						RecievedPacket = Complete;
						print("Packet completed\n\r");
					 }
				 break;
				 
				 case Complete:
					__NOP();
				 break;
			 }
		}
	}
}
/********************************************************************/
// Static functions
/********************************************************************/
// This function combined all init functions
static inline void SysInit(void)
{
	// Init periphs
	CLK_Init();
	GPIO_Init();
	USART0_Init();
	USART1_Init();
	TIM0_Init();
	TIM1_Init();
	// Clear all of buffers
	Clear(&RxBuf);
	Clear(&TxBuf);
}
