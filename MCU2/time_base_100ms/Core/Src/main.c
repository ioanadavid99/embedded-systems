

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

//void SystemClockConfig(void);
void SystemClockConfig(uint8_t clock_freq);
void Error_handler(void);
void TIMER6_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htimer6;	// handler of timer6

int main(void){
	HAL_Init();
	//SystemClockConfig();
	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);	// we do this if we want to use the processor peripherals for this
	GPIO_Init();
	TIMER6_Init();
	// going to toggle the LED every 100ms

	/****** for interrupts ******/
	HAL_TIM_Base_Start_IT(&htimer6);

	/****** for polling ******/
	// now let's start the timer!
	// polling - not good for low power implementation
	/*HAL_TIM_Base_Start(&htimer6);		// have three different options - normal, IT, or DMA
										// normal just polls

	while(1) {
	// loop until the update event flag is set
		while(!(TIM6->SR & TIM_SR_UIF));	// update interrupt flag - until that happens, stay in the loop
		TIM6-> SR = 0;						// clearing the shift register
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	*/

	// timer will be on independent of the processor

	//while(1);								// not required

	return 0;
}

//void SystemClockConfig(void) { }
void SystemClockConfig(uint8_t clock_freq) {
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

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

void Error_handler(void){
	while(1);
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

void TIMER6_Init(void){
	htimer6.Instance = TIM6;
	// can only configure the period & prescaler for a basic timer
	htimer6.Init.Prescaler = 24;				// cannot change counter mode for the basic timer, plus some other settings are unavailable
	htimer6.Init.Period = 64000-1;				// need 1 less to account for the additional clock cycle when it generates an interrupt
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK) {
		Error_handler();
	}
}

// this is defined elsewhere - but has the weak attribute so we are overwriting it
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);

}
