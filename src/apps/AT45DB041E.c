#include "stm32f10x.h"

static inline void delay(void)
{
	__DSB();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}

uint8_t SPI_SendReceiveByte(uint8_t dat)
{
	uint8_t tmp = 0;

	for(int i = 0; i < 8; i++)
	{
		GPIOD->BRR = 2;
		if(dat & 0x80)
		{
			GPIOD->BSRR = 1;
		}
		else
		{
			GPIOD->BRR = 1;
		}
		delay();

		GPIOD->BSRR = 2;
		tmp <<= 1;
		tmp |= ((GPIOD->IDR & 0x08ul) >> 3);
		dat <<= 1;
		delay();
	}
	GPIOD->BRR = 2;
	delay();

	return tmp;
}

void AT45DB041E_CheckBusy(void)
{
	uint32_t cnt = 0;
	uint8_t tmp = 0;

	GPIOD->BRR = 1 << 2;
	delay();

	do{
		SPI_SendReceiveByte(0xd7);
		tmp = SPI_SendReceiveByte(0xff);
		delay();
		cnt++;
	}while((cnt < 0xffff) && (tmp & 0x80));

	GPIOD->BSRR = 1 << 2;
	delay();
}

void AT45DB041E_Read(uint32_t addr, uint8_t *buf, uint32_t size)
{
	GPIOD->BRR = 1 << 2;
	delay();

	SPI_SendReceiveByte(0xe8);
	SPI_SendReceiveByte((addr >> 16) & 0xfful);
	SPI_SendReceiveByte((addr >> 8) & 0xfful);
	SPI_SendReceiveByte(addr & 0xfful);
	SPI_SendReceiveByte(0xff);
	SPI_SendReceiveByte(0xff);
	SPI_SendReceiveByte(0xff);
	SPI_SendReceiveByte(0xff);

	for(int i = 0; i < size; i++)
	{
		*buf++ = SPI_SendReceiveByte(0xff);
	}

	GPIOD->BSRR = 1 << 2;
	delay();
}

void AT45DB041E_Write(uint32_t addr, uint8_t *buf, uint32_t size)
{
	GPIOD->BRR = 1 << 2;
	delay();

	SPI_SendReceiveByte(0x84);
	SPI_SendReceiveByte(0);
	SPI_SendReceiveByte(0);
	SPI_SendReceiveByte(0);

	for(int i = 0; i < size; i++)
	{
		SPI_SendReceiveByte(*buf++);
	}

	GPIOD->BSRR = 1 << 2;
	delay();

	AT45DB041E_CheckBusy();

	GPIOD->BRR = 1 << 2;
	delay();

	SPI_SendReceiveByte(0x83);
	SPI_SendReceiveByte((addr >> 16) & 0xfful);
	SPI_SendReceiveByte((addr >> 8) & 0xfful);
	SPI_SendReceiveByte(addr & 0xfful);

	GPIOD->BSRR = 1 << 2;
	delay();
}

void init_at45db041e(void)
{
	GPIOD->BRR = 1 << 2;
	delay();

	SPI_SendReceiveByte(0x3d);
	SPI_SendReceiveByte(0x2a);
	SPI_SendReceiveByte(0x80);
	SPI_SendReceiveByte(0xa6);

	GPIOD->BSRR = 1 << 2;
	delay();
	AT45DB041E_CheckBusy();

	GPIOD->BRR = 1 << 2;
	delay();

	SPI_SendReceiveByte(0x3d);
	SPI_SendReceiveByte(0x2a);
	SPI_SendReceiveByte(0x7f);
	SPI_SendReceiveByte(0x9a);

	GPIOD->BSRR = 1 << 2;
	delay();
	AT45DB041E_CheckBusy();
}
