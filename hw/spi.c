#include <pic32mx.h>
#include "hw/spi.h"

void spi_initialize() {
	/* Set PBCLK to 80 MHz. Recommended in Lab 3 and OLED display example. */
	SYSKEY = 0xAA996655;
	SYSKEY = 0x556699AA;

	while(OSCCON & (1 << 21)); /* Divisor switch in progress? */
	OSCCONCLR = (1 << 20) | (1 << 19); /* Clear bits 20-19 in OSCCON. */
	while(OSCCON & (1 << 21)); /* Divisor switch in progress? */

	SYSKEY = 0;

	/* Initialize port 2, used by OLED display. */
	SPI2CON = 0;

	SPI2BRG = 4; /* Baud rate divisor 4. */
	SPI2STATCLR = PIC32_SPISTAT_SPIROV;

	SPI2CONSET = PIC32_SPICON_MSTEN; /* Enable master mode. */
	SPI2CONSET = PIC32_SPICON_CKP; /* Clock polarity. */
	SPI2CONSET = PIC32_SPICON_ON; /* Enable SPI port 2. */
}

uint8_t spi2_send(uint8_t data) {
	while(~SPI2STAT & PIC32_SPISTAT_SPITBE); /* Transmit buffer not empty? */
	SPI2BUF = data;
	while(~SPI2STAT & PIC32_SPISTAT_SPIRBF); /* Recieve buffer empty? */

	return SPI2BUF;
}