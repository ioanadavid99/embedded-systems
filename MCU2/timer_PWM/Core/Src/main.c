

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

//void SystemClockConfig(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void Error_handler(void);
void TIMER2_Init(void);
void GPIO_Init(void);
void UART2_Init(void);

TIM_HandleTypeDef htimer2;	// handler of timer2
UART_HandleTypeDef huart2;

int main(void){
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();

	if(HAL_TIM_PWM_Start(&himer2, TIM_CHANNEL_1) != HAL_OK) {
		Error_handler();
	}

	if(HAL_TIM_PWM_Start(&himer2, TIM_CHANNEL_2) != HAL_OK) {
		Error_handler();
	}

	if(HAL_TIM_PWM_Start(&himer2, TIM_CHANNEL_3) != HAL_OK) {
		Error_handler();
	}

	if(HAL_TIM_PWM_Start(&himer2, TIM_CHANNEL_4) != HAL_OK) {
		Error_handler();
	}
	while(1);
	return 0;
}

//void SystemClockConfig(void) { }
void SystemClock_Config_HSE(uint8_t clock_freq) {
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	uint8_t flash_latency = 0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_init.HSIState = RCC_HSI_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;			// using the HSE instead - gives better measurements

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 8;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
	     break;

	  default:
	   return ;
	 }

		if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK) {
			Error_handler();
		}

		if (HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2) != HAL_OK) {
			Error_handler();
		}

	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}

void TIMER2_Init(void) {
	TIM_OC_InitTypeDef tim2PWM_Config;
	htimer2.Instance = TIM2;
	// produces a period of one second
	htimer2.Init.Period = 10000-1;
	htimer2.Init.Prescaler = 4999;
	if(HAL_TIM_PWM_Init(&htimer2) != HAL_OK) {
		Error_handler;
	}

	memset(&tim2PWM_Config, 0, sizeof(tim2PWM_Config));	// initializes all of the member elements to 0
	// good practice to do in case there are any issues seeing this is a local variable

	tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 25) / 100;	// 25% duty cycle

	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_1) != HAL_OK) {
		Error_handler();
	}
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 45) / 100;	// 45% duty cycle
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_2) != HAL_OK) {
		Error_handler();
	}
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 75) / 100;	// 75% duty cycle
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_3) != HAL_OK) {
		Error_handler();
	}
	tim2PWM_Config.Pulse = (htimer2.Init.Period * 95) / 100;	// 95% duty cycle
	if(HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_4) != HAL_OK) {
		Error_handler();
	}
}


void Error_handler(void){
	while(1);	// infinite loop :-)
}

void GPIO_Init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();		// enable the clock
	GPIO_InitTypeDef ledgpio;
	// initializing the pin
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;	// push pull
	ledgpio.Pull = GPIO_NOPULL;			// this is by default - don't need to configure
	HAL_GPIO_Init(GPIOA, &ledgpio);		// initializes things

}


void UART2_Init(void) {
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_handler();
	}
}


