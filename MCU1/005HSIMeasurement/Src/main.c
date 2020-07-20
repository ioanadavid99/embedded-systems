/**
  ******************************************************************************
  * @file    main.c
  * @author  Ioana David
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>

#define ADC_BASE_ADDR		0x40012000UL		// UL - unsigned long
#define ADC_CR1_REG_OFFSET	0x04
#define ADC_CR1_REG_ADDR	(ADC_BASE_ADDR + ADC_CR1_REG_OFFSET)
#define	RCC_BASE_ADDR		0x40023800UL
#define	RCC_APB2_ENR_OFFSET	0x44UL
#define RCC_APB2_ENR_ADDR	(RCC_BASE_ADDR + RCC_ABP2_ENR_OFFSET)
#define RCC_CFGR_REG_OFFSET	0x08UL
#define	RCC_CFGR_REG_ADDR	(RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)
#define GPIOA_BASE_ADDR		0x40020000UL

int main(void)
{

	// 1. configure the RCC clock configuration register
	*pRCC_cfgr &= ~(0x3 << 21)		// clear 21 & 22 bit positions

	// configure the MC01 prescaler
	*pRCC_cfgr |= (1 << 25);
	*pRCC_cfgr |= (1 << 26);

	// 2. configure PA8 to AF0 mode to behave as MC01 signal
		// a) enable the peripheral clock for GPIOA peripheral
		uint32_t *pRCC_ahb1_enr = (uint32_t*)(RCC_BASE_ADDR + 0x30);
		*pRCC_ahb1_enr |= (1 << 0); 	// enable the GPIOA peripheral clock

		// b) configure the mode of the GPIOA pin 8 as alternate function mode
		uint32_t *pGPIOAmodeReg = (uint32_t*)(GPIOA_BASE_ADDR + 00);
		*pGPIOAmodeReg &= ~ (0x3 << 16); // clear
		*pGPIOAmodeReg |= (0x2 << 16);	// set

		// c) configure the alternate function register to set the mode 0 for PA8
		uint32_t *pGPIOA_altFcnHighReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x24)
		*pGPIOA_altFcnHighReg &= (0xf << 0);

	for(;;);
}
