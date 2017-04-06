#include "stm32f10x.h"
#include <string.h>
#include "GlobalFunc.h"

extern void Message_Post(uint8_t cmd, uint8_t *addr, uint32_t size);
static int GetMessage(void);

static uint8_t SerialBuf[256] = {0};
static uint8_t SerialIndex = 0;
static uint8_t Message[256] = {0};
static uint8_t MessageLength = 0;

void USART1_IRQHandler(void)
{
	USART1->SR = 0x3df;
	SerialBuf[SerialIndex] = USART1->DR;
	SerialIndex++;
//	SerialIndex &= 0x3fu;
	TIM7->CNT = 0;
	TIM7->SR = 0;
	TIM7->CR1 = 0x0d;
}

void TIM7_IRQHandler(void)
{
	TIM7->SR = 0;
	TIM7->CR1 = 0;
	GetMessage();
	Message_Post(Message[0], Message + 1, MessageLength - 1);
	memset(SerialBuf, 0, sizeof(SerialBuf));
	SerialBuf[0] = 0;
	SerialIndex = 0;
	Message[0] = 0;
	MessageLength = 0;
	__DSB();
}

static int GetMessage(void)
{
	uint8_t cnt = SerialIndex;
	uint8_t lrc = 0;
	int Index;
	int k;


	if(cnt < 6)
	{
		return -3;
	}

	for(Index = 0; Index < cnt - 2; Index++)
	{
		if((SerialBuf[Index] == 0x7b)/* && (SerialBuf[Index + 1] < 200)*/ && (SerialBuf[Index + SerialBuf[Index + 1] - 1] == 0x7d))
		{
			break;
		}
	}
	if(Index >= 128)	return -1;

	cnt = SerialBuf[Index + 1] - 2;
	MessageLength = cnt;

	for(k = Index + 1; k < cnt; k++)
	{
		lrc += SerialBuf[k];
	}
	if(SerialBuf[k] != lrc)		return -2;

	MessageLength = SerialBuf[Index + 1] - 5;
	memcpy(Message, &SerialBuf[Index + 3], MessageLength);

	return 0;
}

int SendChar(int ch)
{
	while((USART1->SR & 0x80) == 0);
	USART1->DR = ch;
	return ch;
}

uint8_t SendBuffer(uint8_t *buf, uint32_t size)
{
	uint8_t lrc = 0;

	for(int index = 0; index < size; index++)
	{
		lrc += *buf;
		SendChar(*buf++);
	}

	return lrc;
}

void SendACK(void)
{
	SendChar(0x7b);
	SendChar(0x06);
	SendChar(0x00);
	SendChar(0x01);
	SendChar(0x07);
	SendChar(0x7d);
	delay();
}
