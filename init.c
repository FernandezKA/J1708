#include "main.h"

void GPIO_Init(void){
	RCU_APB2EN|=RCU_APB2EN_PCEN;
	gpio_init(GPIOC, GPIO_MODE_OUT_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_13);
}
