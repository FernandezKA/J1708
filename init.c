#include "main.h"

void CLK_Init(void)
{
	// rcu_deinit();
	// rcu_pll_config(RCU_PLLSRC_IRC8M_DIV2, RCU_PLL2_MUL16);
	// rcu_system_clock_source_config(RCU_CKSYSSRC_IRCM8M);
	RCU_APB2EN |= RCU_APB2EN_PCEN | RCU_APB2EN_PAEN;
	rcu_periph_clock_enable(RCU_USBD);
	rcu_ckout0_config(RCU_CKOUT0SRC_CKSYS);
	rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1_5);
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_13); // It's led for indicate activity
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);	// usb
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);	// usb
}

void GPIO_Init(void)
{
	RCU_APB2EN |= RCU_APB2EN_PCEN;
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_13); // It's led for indicate activity
}
// This function init USART0 for communication with PC
void USART0_Init(void)
{
	RCU_APB2EN |= RCU_APB2EN_USART0EN;
	RCU_APB2EN |= RCU_APB2EN_PAEN;
	usart_deinit(USART0);
	usart_baudrate_set(USART0, 115200UL);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	// usart_interrupt_enable(USART0, USART_INT_TBE);
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	gpio_afio_deinit();
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	usart_enable(USART0);
}
// This function init USART1 for J1708 bus
void USART1_Init(void)
{
	RCU_APB1EN |= RCU_APB1EN_USART1EN;
	RCU_APB2EN |= RCU_APB2EN_PAEN;
	usart_deinit(USART1);
	usart_baudrate_set(USART1, 9600UL);
	usart_parity_config(USART1, USART_PM_NONE);
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	// usart_interrupt_enable(USART0, USART_INT_TBE);
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	gpio_afio_deinit();
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	usart_enable(USART1);
}

// This function init TIM0 for j1708 arbitrate
// Each clock is 1 uS
void TIM0_Init(void)
{
	RCU_APB2EN |= RCU_APB2EN_TIMER0EN;
	timer_deinit(TIMER0);
	timer_parameter_struct tim0;
	tim0.prescaler = 1070; // 0.01 mS for each step
	tim0.alignedmode = TIMER_COUNTER_EDGE;
	tim0.counterdirection = TIMER_COUNTER_UP;
	tim0.period = 100000; // About 0.81 sec per UIF IRQ
	timer_init(TIMER0, &tim0);
	timer_interrupt_enable(TIMER0, TIMER_INT_UP); // Interrrupt at overflow
	timer_enable(TIMER0);
}
// This function init TIM1 for device activity and general timing definition
void TIM1_Init(void)
{
	RCU_APB1EN |= RCU_APB1EN_TIMER1EN;
	timer_deinit(TIMER1);
	timer_deinit(TIMER1);
	timer_parameter_struct tim1;
	tim1.prescaler = 10700; // 0.01 mS for each step
	tim1.alignedmode = TIMER_COUNTER_EDGE;
	tim1.counterdirection = TIMER_COUNTER_UP;
	tim1.period = 5000; // About 0.81 sec per UIF IRQ
	timer_init(TIMER1, &tim1);
	timer_interrupt_enable(TIMER1, TIMER_INT_UP); // Interrrupt at overflow
	timer_enable(TIMER1);
}
