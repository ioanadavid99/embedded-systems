/**
  ******************************************************************************
  * @file    main.c
  * @author  Ioana David
  * @version V1.0
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>

#define	RCC_BASE_ADDR		0x40023800UL
#define RCC_CFGR_REG_OFFSET	0x08UL
#define RCC_CR_REG_OFFSET	0x00UL
#define	RCC_CFGR_REG_ADDR	(RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)
#define RCC_CR_REG_ADDR		(RCC_BASE_ADDR + RCC_CR_REG_OFFSET)
#define GPIOA_BASE_ADDR		0x40020000UL


int main(void)
{
	/*
	 * 1. enable the HSE clock
	 * 2. wait until the HSE clock stabilizes
	 * 3. switch the system clock to HSE
	 */

	// NUCLEO BOARD DOES NOT HAVE INTERNAL CRYSTAL - have to enable the bypass register

	uint32_t *pRCC_cfgrReg = (uint32_t*) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_crReg = (uint32_t*) RCC_CR_REG_ADDR;

	// 1. enable the HSE clock using the HSEON bit
	*pRCC_crReg |= (1 << 16);

	// 2.wait until the HSE clock stabilizes
	while(! (*pRCC_crReg & (1 << 17)));

	// 3. switch the system clock to HSE
	*pRCC_cfgrReg |= (1 << 0);

	for(;;);
}
