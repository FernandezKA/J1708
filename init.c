#include "main.h"

void CLK_Init(void){
	rcu_deinit();
	
}

void GPIO_Init(void){
	RCU_APB2EN|=RCU_APB2EN_PCEN;
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_13);
}

void USART0_Init(void){
	RCU_APB2EN|=RCU_APB2EN_USART0EN;
	RCU_APB2EN|=RCU_APB2EN_PAEN;
	usart_deinit(USART0);
	usart_baudrate_set(USART0, 19200UL);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	gpio_afio_deinit();
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	usart_enable(USART0);
}
