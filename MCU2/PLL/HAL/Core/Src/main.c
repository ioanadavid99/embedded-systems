/*
 * main.c
 *
 *  Created on: Jul 25, 2020
 *      Author: idavi
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "string.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t data);

UART_HandleTypeDef huart2;	// handle of UART2
char *user_data = "The application is running\r\n";

int main(void){
	HAL_Init();	// has to be the first function invoked
	SystemClockConfig();
	UART2_Init();
	// transmit over UART the user_data
	HAL_UART_Transmit(&huart2, (uint8_t*) user_data, (uint16_t) strlen(user_data), HAL_MAX_DELAY); // 4th argument indicates how long the blocking call should wait before announcing timeout

	// now we want to receive
	uint8_t received_data;
	uint8_t data_buffer[100];	// data buffer of 100 characters
	uint32_t count = 0;
	// read one byte of data at a time
	while(1) {
		HAL_UART_Receive(&huart2, &received_data, 1, HAL_MAX_DELAY);	// code is blocked until a byte is received
		if(recevied_data == '\r')	// this is the end
			break;
		else
			data_buffer[count++] = convert_to_capital(received_data);
	}

	data_buffer[count++] = 'r';
	HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);

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

uint8_t convert_to_capital(uint8_t data){
	if(data >= 'a' && data <= 'z') { 		// data is lowercase
		data -= 32; 						// convert to a capital here
	}

	return data;
}

void Error_handler(void){
	while(1);
}
