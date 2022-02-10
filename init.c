#include "main.h"

void CLK_Init(void){
	rcu_deinit();
	
}

void GPIO_Init(void){
	RCU_APB2EN|=RCU_APB2EN_PCEN;
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_13);
}
//This function init USART0 for communication with PC
void USART0_Init(void){
	RCU_APB2EN|=RCU_APB2EN_USART0EN;
	RCU_APB2EN|=RCU_APB2EN_PAEN;
	usart_deinit(USART0);
	usart_baudrate_set(USART0, 115200UL);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	//usart_interrupt_enable(USART0, USART_INT_TBE);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	gpio_afio_deinit();
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	usart_enable(USART0);

}
//This function init USART1 for J1708 bus
void USART1_Init(void){
	RCU_APB1EN|=RCU_APB1EN_USART1EN;
	RCU_APB2EN|=RCU_APB2EN_PAEN;
	usart_deinit(USART1);
	usart_baudrate_set(USART1, 9600UL);
	usart_parity_config(USART1, USART_PM_NONE);
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	//usart_interrupt_enable(USART0, USART_INT_TBE);
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	gpio_afio_deinit();
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	usart_enable(USART1);
}

//This function init TIM0 for timing definition
void TIM0_Init(void){
	timer_deinit(TIMER0);
	timer_parameter_struct Tim0; 
	Tim0.prescaler = 48;
	
}
