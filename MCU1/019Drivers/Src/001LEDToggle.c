/*
 * 001LEDToggle.c
 *
 *  Created on: Apr. 21, 2020
 *      Author: idavi
 */
#include "019Drivers.h"
// #include "019Drivers_gpio_driver.h"		// included this in the main header file - no longer need to include it here

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);
}

int main(void){
	GPIO_Handle_t gpioLED;
	gpioLED.pGPIOx = GPIOA;
	gpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	gpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpioLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	/* 	pull-up/pull-down configuration
	 *	gpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	 *	gpioLED.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_NO_PUPD;		// might be PU
	 */

	// open drain - need an external resistor
	gpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	gpioLED.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);		// initializing the clock
	GPIO_Init(&gpioLED);

	while(1){
		GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_5);
		delay();
	}
	return 0;
}

