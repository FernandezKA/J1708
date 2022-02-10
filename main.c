#include "main.h"

void SysInit(void);
//Main function
int main(){
	SysInit();
	for(;;){
		for(uint32_t i = 0; i < 0xFFFFFF; ++i){__NOP();}
		gpio_bit_set(GPIOC, GPIO_PIN_13);
		for(uint32_t i = 0; i < 0xFFFFFF; ++i){__NOP();}
		gpio_bit_reset(GPIOC, GPIO_PIN_13);
	}
}
//This function combined all of init functions
void SysInit(void){
	GPIO_Init();
}
