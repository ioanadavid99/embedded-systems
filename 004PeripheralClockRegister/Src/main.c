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
#define RCC_APB2_ENR_ADDR	(RCC_BASES_ADDR + RCC_ABP2_ENR_OFFSET)

int main(void)
{
	uint32_t *pADC_ctr_reg = (uint32_t*) ADC_CR1_REG_ADDR;
	uint32_t *pRCC_apb2_enr = (uint32_t*) RCC_APB2_ENR_ADDR;

	// enable the peripheral clock for ADC1
	*pRCC_apb2_enr |= (1<<8);

	// modify the ADC CR1 register
	*pADC_ctr_reg |= (1 << 8);

	for(;;);
}
