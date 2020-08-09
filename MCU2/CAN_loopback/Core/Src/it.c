/*
 * it.c
 *
 *  Created on: Aug 4, 2020
 *      Author: idavi
 */

#include "main.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
