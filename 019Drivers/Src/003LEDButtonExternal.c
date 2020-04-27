/*
 * 003LEDButtonExternal.c
 *
 *  Created on: Apr. 22, 2020
 *      Author: idavi
 */

/*
 *
 * NB: PA14 won't work, it can't be used as a GPIO - use something else (reference the manual)
 */

#include "019Drivers.h"
// #include "019Drivers_gpio_driver.h"		// included this in the main header file - no longer need to include it here

void delay(void){
	for(uint32_t i = 0; i < 5000; i++);
}

int main(void){
	GPIO_Handle_t gpioLED, gpioBTN;

	// LED configuration
	// connecting an EXTERNAL LED to PA14
	gpioLED.pGPIOx = GPIOA;
	gpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	gpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	gpioLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	/* 	pull-up/pull-down configuration
	 *	gpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	 *	gpioLED.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_NO_PUPD;		// might be PU
	 */

	// open drain - need an external resistor
	gpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpioLED.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA,ENABLE);
	GPIO_Init(&gpioLED);

	// configuration for the button below
	// EXTERNAL button at PB12
	gpioBTN.pGPIOx = GPIOB;
	gpioBTN.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	gpioBTN.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	gpioBTN.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	gpioBTN.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	gpioBTN.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOB, ENABLE);		// initializing the clock
	GPIO_Init(&gpioBTN);
	uint8_t pinValue;
	while(1){
		// this toggle isn't going to be flawless - reads will not always be synchronous to when you're pushing the button
		// ie. you have to push the button RIGHT when it reads for it to pick up
		// de-bouncing issues
		pinValue = GPIO_ReadFromInputPin(GPIOB,GPIO_PIN_NO_12);
		if(pinValue == 1){		// if the button is pressed - it will be low.
			GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_12, RESET);
			//delay();
		}
		else {
			GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_14);
			delay();
		}
	}
	return 0;
}
