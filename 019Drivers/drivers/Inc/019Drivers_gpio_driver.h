/*
 * 019Drivers_gpio_driver.h
 *
 *  Created on: Apr. 19, 2020
 *      Author: idavi
 */

#ifndef INC_019DRIVERS_GPIO_DRIVER_H_
#define INC_019DRIVERS_GPIO_DRIVER_H_

#include "019Drivers.h"

typedef struct{
	uint8_t	GPIO_PinNumber;
	uint8_t GPIO_PinMode;			// possible values from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;			// create documentation + a reference to the section
	uint8_t GPIO_Pin_PuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

// Handle structure for a GPIO pin

typedef struct
{
	// pointer to hold the base address of the GPIO peripheral
	GPIO_RegDef_t *pGPIOx;		// holds the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;	// holds the pin configuration settings
}GPIO_Handle_t;

/********************************************/
// @GPIO_PIN_MODES
// GPIO possible pin modes
#define GPIO_MODE_IN		0
#define	GPIO_MODE_OUT		1
#define	GPIO_MODE_ALTFN		2
#define	GPIO_MODE_ANALOG	3
#define	GPIO_MODE_IN_FT		4	// falling edge
#define	GPIO_MODE_IN_RT		5	// rising edge
#define	GPIO_MODE_I_RFT	6

// output types
#define	GPIO_OP_TYPE_PP		0	// push-pull
#define	GPIO_OP_TYPE_OD		1	// open-drain

// output speeds
#define	GPIO_SPEED_LOW		0
#define	GPIO_SPEED_MED		1
#define	GPIO_SPEED_FAST		2
#define	GPIO_SPEED_HIGH		3

// pull up/down configuration
#define	GPIO_NO_PUPD		0
#define	GPIO_PIN_PU			1
#define	GPIO_PIN_PD			2


// copied from the repo
/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0  				0
#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2  				2
#define GPIO_PIN_NO_3  				3
#define GPIO_PIN_NO_4  				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_8  				8
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10  			10
#define GPIO_PIN_NO_11 				11
#define GPIO_PIN_NO_12  			12
#define GPIO_PIN_NO_13 				13
#define GPIO_PIN_NO_14 				14
#define GPIO_PIN_NO_15 				15


/********************************************/
// APIs supported by the driver
// de/initialize APIs
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);							// de-initialize the register - sending register back to reset state/value

// peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);		// enable/disable peripheral clock for a given base address, EnOrDi = enable/disable the clock

// data read APIs
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);			// returns the content of the register - 16 bits

// data write APIs
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);		// value is either 0 or 1 (set/reset)
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

// interrupt APIs
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi);			// interrupt configuration
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_019DRIVERS_GPIO_DRIVER_H_ */
