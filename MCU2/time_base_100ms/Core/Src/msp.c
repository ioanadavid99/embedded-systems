/*
 * msp.c
 *
 *  Created on: Jul 27, 2020
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

void HAL_TIME_BASE_MspInit(TIME_HandleTypeDef *htimer){
	// low level initializations

	// 1. enable the clock
	__HAL_RCC_TIM6_CLK_ENABLE();

	// 2. enable IRQ of timer6
	HAL_NVIC_EnableIRQ(TIM6_DAQ_IRQn);	// timer6 & DAC share this IRQ #

	// 3. enable the priority
	HAL_NVIC_SetPriority(TIM6_DAQ_IRQn, 15, 0);

}

