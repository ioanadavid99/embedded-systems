/*
 * main.c
 *
 *  Created on: Jul 25, 2020
 *      Author: idavi
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;

int main(void){
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	char msg[100];

	HAL_Init();

	UART2_Init();

	memset(&osc_init, 0, sizeof(osc_init));
	osc_init.OscillatorType = RCC_OSCILATTORTYPE_HSE;
	// don't have an actual crystal on the board - so we are using the bypass method
	// pin 8 is linked to this part
	osc_init.HSEState = RCC_HSE_BYPASS;

	// this returns status
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK) {
			// flash latency is 0 - HCLK is under 30MHz
		Error_handler();
	}

	// disables the HSI - saves some current, thus saves some power
	__HAL_RCC_HSI_DISABLE();

	/*
	sprintf(msg, "SYSCLK: %ld\r\n", HAL_RCC_GetSysClkFreq());
	HAL_UART_Transmit(&huart2, msg, strlen(msg), HAL_MAX_DELAY);
	*/

	// redo the systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	// previous UART init was called for a *different* clock speed

	UART2_Init();

	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK   : %ldHz\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1  : %ldHz\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2  : %ldHz\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	while(1);

	return 0;
}

void UART2_Init(void){
	// want to try to avoid hardcoding as much as possible - use macros
	huart2.Instance = USART2;				// initialize to base address of UART2 peripheral
	huart2.Init.BaudRate = 115200;			// can initialize the baud rate this way
	huart2.Init.WordLength = UART_WORDLENGTH_8B;	// macro already initialized - 8 bits
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	// initializes the UART mode according to the UART_InitTypeDef settings
	if(HAL_UART_Init(&huart2) != HAL_OK) {
		// there is a problem !!!!
		Error_handler();
	}

}

void Error_handler(void)
{
	while(1);
}
