/*
 * it.c
 *
 *  Created on: Jul 27, 2020
 *      Author: idavi
 */


#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

void SysTick_Handler(void){
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

// the following is for the main_it.c file
