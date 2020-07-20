/*
 * 005spi_tx_testing.c
 *
 *  Created on: Apr. 29, 2020
 *      Author: idavi
 */

// need to configure the GPIO pins to behave as  MOSI, SCLK, MISO etc
/*
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 * PB13 -> SPI2_CLK
 * PB12 -> SPI2_NSS
 * alt function mode: 5
 */
#include "019Drivers.h"
#include <string.h>

void SPI2_GPIOInits(void){
	GPIO_Handle_t SPIpins;
	SPIpins.pGPIOx = GPIOB;
	SPIpins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIpins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIpins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIpins.GPIO_PinConfig.GPIO_Pin_PuPdControl = GPIO_NO_PUPD;
	SPIpins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIpins);

	// MOSI
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIpins);

	// MISO
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIpins);

	// NSS
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIpins);
}

void SPI2_Inits(void){
	SPI_Handle_t  SPI2handle;
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;	// 8 MHz clock
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_HIGH;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);

}

int main(void){
	// initialize the pins
	// don't want MISO and NSS because there is no slave
	// configure the control registers for the SPI *before* the SPI peripheral is enabled - will otherwise be communicating & blocking
	SPI2_GPIOInits();	// initializes the GPIO pins to act as SPI pins
	SPI2_Inits();		// peripheral configuration

	// makes NSS signal high internally
	SPI_SSIConfig(SPI2, ENABLE);

	// enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	// create data
	char user_data[] = "Hello world";
	SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

	// while the SPI is not busy
	while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
