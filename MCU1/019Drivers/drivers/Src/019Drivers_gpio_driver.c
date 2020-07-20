/*
 * 019Drivers_gpio_driver.c
 *
 *  Created on: Apr. 19, 2020
 *      Author: idavi
 */

#include "019Drivers_gpio_driver.h"

// don't forget to include the documentation!


void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	/*
	 * PURPOSE:	initializes the GPIO pin (??)
	 * PARAM:	base address of GPIO handle
	 * RETURN:	none
	 */

	/*
	 * steps:
	 * 1. configure the mode of the GPIO pin
	 * 2. configure the speed
	 * 3. configure the pull up/down settings
	 * 4. configure the op-type
	 * 5. (OPTIONAL) configure the alternate functionality
	 */

	uint32_t temp=0;		// temp register
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// TODO figure out what the code below does (how the pointers work etc) - rewatch part of Sec. 25

	// 1. configure the mode of the GPIO pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		// non-interrupt code
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));	// the 2 is there because it is a 2-bit field
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));	// clearing the bits
		pGPIOHandle->pGPIOx->MODER |= temp;																		// be careful when you're changing the contents of registers - use bitwise or
	}
	else {
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IN_FT){
			// configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// clear the RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IN_RT){
			// configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IN_RFT){
			// configure the FTSR and RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		// configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		// enable the EXTI interrupt delivery method using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;

	// 2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;	// clearing the bit fields

	// 3. configure the pull up/down settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_Pin_PuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	// 4. configure the output type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // only one bit field for the output type
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	// 5. configure the alternate function registers
	// AFR[0] - low register | AFR[1] - high register
	// in order to determine which register - pin # % 8 - the result is either 0 or 1, which gives you the register
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
			uint32_t temp1, temp2;
			temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
			temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
			pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));			// clearing
			pGPIOHandle->pGPIOx->AFR[temp1] |= pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2);
	}

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	/*
	 * PURPOSE:
	 * PARAM:
	 * RETURN:
	 */
	if (pGPIOx == GPIOA)
		GPIOA_REG_RESET();
	else if (pGPIOx == GPIOB)
		GPIOB_REG_RESET();
	else if (pGPIOx == GPIOC)
		GPIOC_REG_RESET();
	else if (pGPIOx == GPIOD)
		GPIOD_REG_RESET();
	else if (pGPIOx == GPIOE)
		GPIOE_REG_RESET();
	else if (pGPIOx == GPIOF)
		GPIOF_REG_RESET();
	else if (pGPIOx == GPIOG)
		GPIOG_REG_RESET();
	else if (pGPIOx == GPIOH)
		GPIOH_REG_RESET();
	 else (pGPIOx == GPIOI)
		GPIOI_REG_RESET();
}

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi){
	/*
	 * PURPOSE:			enable/disable peripheral clock for given GPIO port
	 * PARAM 	pGIPOx:	base address of GPIO peripheral
	 * PARAM	EnOrDi:	enable/disable macros
	 * RETURN:			none
	 */

	if(EnOrDi == ENABLE){
		if (pGPIOx == GPIOA)
			GPIOA_PCLK_EN();
		else if (pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
		else if (pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
		else if (pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
		else if (pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
		else if (pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
		else if (pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
		else if (pGPIOx == GPIOH)
			GPIOH_PCLK_EN();
		else if (pGPIOx == GPIOI)
			GPIOI_PCLK_EN();
	}
	else{
		if (pGPIOx == GPIOA)
			GPIOA_PCLK_DI();
		else if (pGPIOx == GPIOB)
			GPIOB_PCLK_DI();
		else if (pGPIOx == GPIOC)
			GPIOC_PCLK_DI();
		else if (pGPIOx == GPIOD)
			GPIOD_PCLK_DI();
		else if (pGPIOx == GPIOE)
			GPIOE_PCLK_DI();
		else if (pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
		else if (pGPIOx == GPIOG)
			GPIOG_PCLK_DI();
		else if (pGPIOx == GPIOH)
			GPIOH_PCLK_DI();
		else if (pGPIOx == GPIOI)
			GPIOI_PCLK_DI();
	}
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	/*
	 * PURPOSE:	returns the contents of the input pin
	 * PARAM:	pGPIOx: the base address of the GPIO register
	 * 			PinNumber: the pin number in which we're interested
	 * RETURN:	0 or 1
	 */

	// have to read the corresponding bit position in the input bit position (??)

	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) && 0x00000001); 	// dereference base address address & select IDR (input data register)
															// shift the bit you want by the pin number and read the LSB. ie. if there is data in pin8, shift to the right 8x to get to the LSB
															// mask all the bits except for the LSB
	return value;

}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	/*
	 * PURPOSE:	returns the value of the input port at the base address specified
	 * PARAM:	pGPIOx: base address of the GPIO register
	 * RETURN:	the value of the input data register
	 */

	uint16_t value;			// 16 because it's a bus (port) instead of a pin
	value = (uint16_t)pGPIOx-> IDR;
	return value;

}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	/*
	 * PURPOSE:				write Value to PinNumber
	 * PARAM:	pGPIOx: 	base address of GPIO port
	 * 			PinNumber:	the pin # we're interested in
	 * 			Value:		value we want to write
	 * RETURN:				none
	 */

	if (Value == GPIO_PIN_SET)
		// write 1 to the output data register at the bit field corresponding to the pin #
		pGPIOx->ODR |= (1 << PinNumber);		// output data register
	else
		// write 0 to clear
		pGPIOx->ODR &= ~(1 << PinNumber);		// clearing the bits
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	/*
	 * PURPOSE:	write Value to the desired GPIO port
	 * PARAM:	pGPIOx: base address of GPIO port
	 * 			Value: 	value to write
	 * RETURN:	none
	 */

	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	/*
	 * PURPOSE:	toggles the output pin
	 * PARAM:	pGPIOx: base address of GPIO port, PinNumber: the pin number we're interested in
	 * RETURN:	none
	 */

	pGPIOx->ODR ^= (1 << PinNumber);	// XOR operation
										// this toggles the required bit field whenever this is called
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnOrDi){
	/*
	 * PURPOSE:
	 * PARAM:
	 * RETURN:
	 */

	// have to configure the NVIC registers to set the priority for a given IRQ number
	// also have to enable the IRQ - by default they are disabled

	if (EnOrDi == ENABLE) {
		if (IRQNumber <= 31)
			*NVIC_ISER0 |= (1 << IRQNumber);
		else if (IRQNumber > 31 && IRQNumber < 64)
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		else if (IRQNumber >= 64 && IRQNumber < 96)
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
	}

	else {
		if(IRQNumber <= 31)
		{
			// program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			// program ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			// program ICER2 register
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	/*
	 * PURPOSE:
	 * PARAM:
	 * RETURN:
	 */

	// 1. determine the IPR register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	// number of priority bits implemented - specific to the microcontroller
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPL);
	// jumping to the corresponding address by adding the offset
	*(NVIC_PR_BASEADDR + (iprx * 4 )) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber){
	/*
	 * PURPOSE:
	 * PARAM:
	 * RETURN:
	 */

	// clear the EXTI PR register corresponding to the pin number
	if(EXTI->PR & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR |= ( 1 << PinNumber);
	}

}
