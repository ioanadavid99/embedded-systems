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
void SystemClock_Config(uint8_t clock_freq);

UART_HandleTypeDef huart2;

int main(void){
	char msg[100];
	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_120_MHZ);	// can just give it a different macro for a different frequency
	UART2_Init();

	// prints out details
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

void SystemClock_Config(uint8_t clock_freq) {
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;							// this is by default on - don't need to do this
	osc_init.HSICalibrationValue = 16;						// 16 is its value after reset - anything higher increases the freq, anything lower decreases it
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;				// don't have an external crystal :-(

	switch(clock_freq) {
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			// configuring the PLLm
			// these are hte different dividers
			// in order to generate 50MHz, need the following settings
			osc_init.PLL.PLLM = 16;	// has to be 16
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \	// this thingy continues the code on the following line
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;	// the sysclk source is the PLL
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_1WS;		// this value is from the datasheet - tells you what the latency has to be
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ = 2;
			osc_init.PLL.PLLR = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;
		}
		/*
		 * in case you want to configure for 180MHz (to use the HSE instead of HSI):
		 * - need a VOS (voltage scale)
		 * - if you want to run the MCU at 180MHz, then the over-drive mode has to be turned on
		 * - otherwise, AHB clock will not reach 180MHz
		 * - voltage or power scale has to be 1
		 * - need to configure this in the PWR CR (control register)
		 * - there are functions to be able to do that before configuring the dividers
		 *
		 * __HAL_RCC_PWR_CLK_ENABLE();
		 * __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
		 * __HAL_PWR_OVERDRIVE_ENABLE();
		 */

		default:
			return;
	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK) {
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK) {
		Error_handler();
	}

	// systick configuration like before
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

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
