/*
 * main_it.c
 *
 *  Created on: Jul 25, 2020
 *      Author: idavi
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

#define		TRUE	1
#define		FALSE	0

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t data);
void HAL_UART_RxCpltCallBack(UART_HandleTypeDef *huart);

UART_HandleTypeDef huart2;	// handle of UART2
char *user_data = "The application is running\r\n";
uint8_t data_buffer[100];
uint8_t received_data;
uint32_t count = 0;
uint8_t reception_complete = FALSE;

int main(void){
	HAL_Init();	// has to be the first function invoked
	SystemClockConfig();
	UART2_Init();
	// transmit over UART the user_data
	HAL_UART_Transmit(&huart2, (uint8_t*) user_data, (uint16_t) strlen(user_data), HAL_MAX_DELAY); // 4th argument indicates how long the blocking call should wait before announcing timeout

	// store the received data
	while(reception_complete != TRUE)
		HAL_UART_Receive_IT(&huart2, &received_data, 1);		// non-blocking function


	while(1);
	return 0;
}

void SystemClockConfig(void) {

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

void HAL_UART_RxCpltCallBack(UART_HandleTypeDef *huart){
	// need to modify this function ONLY when you want callbacks
	// weak function belongs in a different file - but this is OK to modify bc the other is a weak function

	if(received_data == '\r') {
		reception_complete = TRUE;
		data_bufffer[count++] = '\r'
		HAL_UART_Transmit(huart, data_buffer, count, HAL_MAX_DELAY);
	}
	else {

	}
}

uint8_t convert_to_capital(uint8_t data){
	if(data >= 'a' && data <= 'z') { 		// data is lowercase
		data -= 32; 						// convert to a capital here
	}

	return data;
}

void Error_handler(void){
	while(1);
}

