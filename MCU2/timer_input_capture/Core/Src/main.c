

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

//void SystemClockConfig(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void Error_handler(void);
void TIMER6_Init(void);
void GPIO_Init(void);
void UART2_Init(void);
void LSE_Configuration(void);
void HAL_TIME_IC_CaptureCallback(TIM_HandleTypeDef *htim);

TIM_HandleTypeDef htimer2;	// handler of timer2
TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart2;

uint32_t input_captures[2] = {0};
uint8_t count=1;

volatile uint8_t is_capture_done = FALSE;

int main(void){
	uint32_t capture_difference = 0;
	double timer2_cnt_freq = 0 ;
	double timer2_cnt_res = 0;
	double user_signal_time_period = 0;
	double signal_freq = 0;
	char user_msg[100];

	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);	// we do this if we want to use the processor peripherals for this
	GPIO_Init();
	TIMER2_Init();
	LSE_Configuration();

	while(1) {
		if(is_capture_done) {
			if(input_captures[1] > input_captures[0]) {
				capture_difference = input_captures[1] - input_captures[0];
			}
			else {
				capture_difference = (0xFFFFFFFF - input_captures[0]) + input_captures[1];
			}
		timer2_cnt_freq = (HAL_RCC_GetPclkFreq() * 2) / (htimer2.Init.Prescaler + 1);
		timer2_cnt_res = 1 / timer2_cnt_freq;
		user_signal_time_period = capture_difference * timer2_cnt_res;
		user_signal_freq = 1 / user_signal_time_period;
		sprintf(user_msg, "Frequency of the signal applied = %.2f Hz\r\n", user_signal_freq);
		HAL_UART_Transmit(&huart2,(uint8_t*)user_msg,strlen(user_msg),HAL_MAX_DELAY);
		is_capture_done = FALSE;
		}
	}

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
	TIM_IC_InitTypeDef timer2IC_Config;
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP; 	// don't need to initialize - automatically counts up
	htimer2.Init.Period = 0xFFFFFFFF; 				// max as described in the reference manual
	htimer2.Init.Prescaler = 1;						// will be running at a smaller frequency than the processor - at 25MHz
	if (HAL_TIM_IC_INIT(&htimer2) != HAL_OK) {
		Error_handler();
	}
	// configuring the input channel
	timer2IC_Config.ICFilter = 0;
	timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;	// want to generate an event on the rising edge
	timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK) {
		Error_handler();
	}
}

void TIMER6_Init(void){
	htimer6.Instance = TIM6;
	// can only configure the period & prescaler for a basic timer
	htimer6.Init.Prescaler = 9;				// cannot change counter mode for the basic timer, plus some other settings are unavailable
	htimer6.Init.Period = 50-1;				// need 1 less to account for the additional clock cycle when it generates an interrupt
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK) {
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

// this is defined elsewhere - but has the weak attribute so we are overwriting it
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

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

void LSE_Configuration(void) {
	// activate the crystal oscillator
	RCC_OscInitTypeDef Osc_Init;
	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	Osc_Init.LSEState = RCC_LSE_ON;
	if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK) {	// making sure that everything is OK
		Error_handler();
	}
	// configuring the thingy
	// produce the LSE clock on the MCO1 pin of the MCU- happens to be PA8
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI,RCC_MCODIV_4);	// MCO = microcontroller output
}

void HAL_TIME_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if(is_capture_done == FALSE) {
		if(count == 1) {
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);	// reads the CCR1 register
			count++;
		}
		else if(count == 2) {
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);
			count = 1;
			is_capture_done = TRUE;
		}
	}
}
