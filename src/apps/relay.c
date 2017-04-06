#include "stm32f10x.h"
#include "GlobalFunc.h"
#include <stdio.h>

void RelayControl(void *addr)
{
	uint8_t ch = *(uint8_t *)addr;

	if(ch & 0x01)
	{
		GPIOA->BSRR = 1 << 12;
	}
	else
	{
		GPIOA->BRR = 1 << 12;
	}

	if(ch & 0x02)
	{
		GPIOB->BSRR = 1 << 8;
	}
	else
	{
		GPIOB->BRR = 1 << 8;
	}

	if(ch & 0x04)
	{
		GPIOA->BSRR = 1 << 8;
	}
	else
	{
		GPIOA->BRR = 1 << 8;
	}

	SendACK();
	delay();

	printf("%s\n", __func__);
}
