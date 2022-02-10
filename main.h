#include <stdint.h>
//MCU lib include
#include <gd32f10x.h>
#include <gd32f10x_gpio.h>
#include <gd32f10x_rcu.h>
#include <gd32f10x_timer.h>
#include <gd32f10x_misc.h>
//User inludes
#include "j1708.h"
#include "uart.h"
//User init functions init

void CLK_Init(void);
void GPIO_Init(void);
void USART0_Init(void);
void USART1_Init(void);
void TIM1_Init(void);
