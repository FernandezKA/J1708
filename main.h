#include <stdint.h>
//MCU lib include
#include <gd32f10x.h>
#include <gd32f10x_gpio.h>
#include <gd32f10x_rcu.h>
#include <gd32f10x_timer.h>
#include <gd32f10x_misc.h>
#include <gd32f10x_usart.h>
//User inludes
#include "j1708.h"
#include "uart.h"
#include "fifo.h"
//User typedefs 
typedef struct fifo FIFO;
typedef struct j1708 J1708;
//User defines 
#define PC_UART USART0
#define J1708_UART USART1

//User init functions init

void CLK_Init(void);
void GPIO_Init(void);
void USART0_Init(void);
void USART1_Init(void);
void TIM0_Init(void);
void TIM1_Init(void);

//User variables
//For communicate with PC
extern FIFO RxBuf;
extern FIFO TxBuf;
//For J1708 communicate
extern FIFO J1708_RxBuf;
extern FIFO J1708_TxBuf;
//
extern J1708 RxStruct;
