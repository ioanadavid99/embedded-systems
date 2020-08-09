/*
 * it.c
 *
 *  Created on: Jul 27, 2020
 *      Author: idavi
 */


#include "stm32f4xx_hal.h"
#include "main.h"

extern TIM_HandleTypeDef htimer2;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}
