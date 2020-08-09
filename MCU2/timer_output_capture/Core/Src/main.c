

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

//void SystemClockConfig(void);
void SystemClock_Config_HSE(uint8_t clock_freq);
void Error_handler(void);
void TIMER2_Init(void);
void GPIO_Init(void);
void UART2_Init(void);
void LSE_Configuration(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

TIM_HandleTypeDef htimer2;	// handler of timer2
UART_HandleTypeDef huart2;

volatile uint32_t pulse1_value = 25000; //to produce 500Hz
volatile uint32_t pulse2_value = 12500; //to produce 1000HZ
volatile uint32_t pulse3_value = 6250;  //to produce 2000Hz
volatile uint32_t pulse4_value = 3125;  //to produce 4000Hz

uint32_t ccr_content;

int main(void){
	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	UART2_Init();
	TIMER2_Init();
	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK) {	// start the timer in output mode
		Error_handler();
	}
	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK) {	// start the timer in output mode
		Error_handler();
	}
	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK) {	// start the timer in output mode
		Error_handler();
	}
	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK) {	// start the timer in output mode
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
	TIM_OC_InitTypeDef tim2OC_init;
	// empty for the time being
	// have to configure channel as output
	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;	// want this clock to run at 25MHz
	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK){
		Error_handler();
	}

	// have to configure the output channels of the timer
	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;	// polarity is 0 (the number is 0x0..0U
	tim2OC_init.Pulse  = pulse1_value;				// this is the pulse value - calculated depending on the frequency needed at the output channel
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_1) != HAL_OK) {
		Error_handler();
	}

	tim2OC_init.Pulse  = pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_2) != HAL_OK) {
		Error_handler();
	}

	tim2OC_init.Pulse  = pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_3) != HAL_OK) {
		Error_handler();
	}

	tim2OC_init.Pulse  = pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2,&tim2OC_init,TIM_CHANNEL_4) != HAL_OK) {
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

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *him) {
	// channel 1
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+pulse1_value);
	}

	// channel 2
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2,ccr_content+pulse2_value);
	}

	// channel 3
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,ccr_content+pulse3_value);
	}

	// channel 4
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,ccr_content+pulse4_value);
	}
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


