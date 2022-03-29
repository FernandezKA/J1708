#include "main.h"
// User defines
#define PCKT_DELAY 803U
// User global variables
/*******************************************************************************/
// For usbd implementation
extern uint8_t packet_sent, packet_receive;
extern uint32_t receive_length;
extern uint8_t usb_data_buffer[CDC_ACM_DATA_PACKET_SIZE];

usbd_core_handle_struct usb_device_dev =
	{
		.dev_desc = (uint8_t *)&device_descriptor,
		.config_desc = (uint8_t *)&configuration_descriptor,
		.strings = usbd_strings,
		.class_init = cdc_acm_init,
		.class_deinit = cdc_acm_deinit,
		.class_req_handler = cdc_acm_req_handler,
		.class_data_handler = cdc_acm_data_handler};

/*******************************************************************************/

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
static inline void usbd_polling(void);
/*********************************************************************/
int main()
{
	SysInit();
	nvic_irq_enable(USART0_IRQn, 2, 1);	   // For UART0_PC
	nvic_irq_enable(USART1_IRQn, 1, 1);	   // For J1708 UART IRQ
	nvic_irq_enable(TIMER0_UP_IRQn, 2, 2); // For timming definition
	nvic_irq_enable(TIMER1_IRQn, 3, 3);	   // For led indicate activity
	nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
	nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn, 1, 1);
	// User main variables
	static bool isParsed = FALSE;
	static bool is_J1708_Completed = FALSE;
	static uint16_t countDataParsed = 0;
	print("J1708 adapter\n\r");
	print("2022-03-29 ver. 1.0\n\r");
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
		
		usbd_polling();
		
		// This part of code for RS232 -> J1708 parsing
		/********************************************************************/
		/*Input packet struct:
		 *Priority  (1 - 8)
		 *Size      (Max buff size 64U)
		 *MID       (0-68)
		 *Data
		 */
		/*****************************************************************/
		// Get check for packet complete, send on j1708 bus
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
			usart_interrupt_enable(J1708_UART, USART_INT_TBE);
			RecievedPacket = getPriority;
			is_J1708_Completed = FALSE;
			TIMER_CNT(TIMER0) = 0;
		}

		// Get parse Rx UART_PC
		if (GetSize(&RxBuf) != 0)
		{
			switch (RecievedPacket)
			{
			case getPriority:
				TxStruct.Priority = (uint16_t)Pull(&RxBuf);
				if (TxStruct.Priority < 1 || TxStruct.Priority > 8)
				{
					print("Invalid priority. Buffer clean \n\r");
					Clear(&RxBuf);
					RecievedPacket = getPriority;
				}
				else
				{
					RecievedPacket = getSize;
				}
				break;

			case getSize:
				TxStruct.Size = (uint8_t)Pull(&RxBuf);
				if (TxStruct.Size > 64)
				{
					print("Invalid size. Buffer clean \n\r");
					Clear(&RxBuf);
					RecievedPacket = getPriority;
				}
				else
				{
					RecievedPacket = getMID;
				}
				break;

			case getMID:
				TxStruct.MID = (uint8_t)Pull(&RxBuf);
				if (TxStruct.MID > 68)
				{
					print("Invalid MID. Buffer clean \n\r");
					Clear(&RxBuf);
					RecievedPacket = getPriority;
				}
				else
				{
					RecievedPacket = getData;
				}
				break;

			case getData:
				if (countDataParsed < TxStruct.Size - 1)
				{
					TxStruct.Data[countDataParsed++] = (uint8_t)Pull(&RxBuf);
				}
				else
				{
					TxStruct.Data[countDataParsed] = (uint8_t)Pull(&RxBuf);
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
	usbd_core_init(&usb_device_dev);
	//USART0_Init();
	USART1_Init();
	TIM0_Init();
	TIM1_Init();
	// Clear all of buffers
	Clear(&RxBuf);
	Clear(&TxBuf);
}

static inline void usbd_polling(void)
{
	// This part of code parse inout data buff
	/*******************************************************************************/
	// USBD chech buffer
	if (USBD_CONFIGURED == usb_device_dev.status)
	{
		if (1 == packet_receive)
		{
			cdc_acm_data_receive(&usb_device_dev);
		}
		else
		{
			if (0 != receive_length)
			{
				// cdc_acm_data_send(&usb_device_dev, receive_length);
				for (uint8_t i = 0; i < receive_length; ++i)
				{
					Push(&RxBuf, usb_data_buffer[i]);
				}
				receive_length = 0;
			}
		}
	}
}
