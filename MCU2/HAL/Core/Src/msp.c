/*
 * msp.c
 *
 *  Created on: Jul 25, 2020
 *      Author: idavi
 */

#include "stm32f4xx.h"

void HAL_MspInit(void) {
	// low level processor specific inits

	// 1. set up priority grouping
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);		// this is redundant - priority grouping will be 4 anyway

	// 2. enable the required system exceptions
	SCB->SHCSR |= 0x7 << 16;	// enable UsageFault, MemManageFault, and BusFault system exception

	// 3. configure the priority for the system exceptions
	// priority is already 0 (by default) - don't need to configure anything
	// but this is done anyway just to see how it's done
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart){
	GPIO_InitTypeDef gpio_uart;
	// low level init of UART2 peripheral

	// 1. enable clock
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. pin muxing configs
	gpio_uart.Pin = GPIO_PIN_2;
	// needs to be alternate functionality - configuring it to do an alternate function
	gpio_uart.Mode = GPIO_MODE_AF_PP;			// alternate function push pull
	gpio_uart.Pull = GPIO_PULLUP;				// when there is no communication - line is pulled high
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;		// speed of transition from 0 to 1 or vice versa
	gpio_uart.Alternate = GPIO_AF7_USART2;		// have to connect UART Tx function to this pin

	// configuring everything for UART2 Tx
	HAL_GPIO_Init(GPIOA, &gpio_uart); // base address of GPIO A peripheral; address to the structure

	// configuring things for Rx
	gpio_uart.Pin = GPIO_PIN_3;
	// no other changes for Rx
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	// 3. enable the IRQ and set up the priority - NVIC settings
	// helpful for low-power applications as well
	// have to enable interrupt # associated w that peripheral (IRQ #)
	HAL_NVIC_EnableIRQ(USART2_IRQn);	// enables it !! yay
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}
