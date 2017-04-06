#include "stm32f10x.h"
#include "GlobalFunc.h"
#include "datatype.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

extern const int NWaveCoeff;
extern const int16_t WaveCoeff[];

uint16_t voltage[2048 + 128] = {0};
uint16_t current[2048 + 128] = {0};
POWER_T power;
SYSCALIBRATION_T sysCoeff;
static uint8_t State = 0;

void init_coeff(void)
{
	AT45DB041E_Read(0x800, (uint8_t *)&sysCoeff, sizeof(sysCoeff));
}

void ReadSysCoeff(void *addr)
{
	uint8_t lrc = 0;

	SendChar(0x7b);
	lrc = sizeof(SYSCALIBRATION_T) + 6;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x48);
	lrc += 0x48;

	lrc += SendBuffer((uint8_t *)&sysCoeff, sizeof(SYSCALIBRATION_T));

	SendChar(lrc);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}

void WriteSysCoeff(void *addr)
{
	memcpy(&sysCoeff, addr, sizeof(sysCoeff));
	AT45DB041E_Write(0x800, (uint8_t *)&sysCoeff, sizeof(sysCoeff));

	SendACK();

	delay();

	printf("%s\n", __func__);
}

void ReadPower(void *addr)
{
	uint8_t lrc = 0;

	SendChar(0x7b);
	lrc = sizeof(POWER_T) + 6;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x41);
	lrc += 0x41;

	lrc += SendBuffer((uint8_t *)&power, sizeof(POWER_T));

	SendChar(lrc);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}

uint32_t CalculatePowerb(uint16_t *u, uint16_t *i, uint32_t size)
{
	uint64_t mac = 0;

	for(int index = 0; index < size; index++)
	{
		mac += u[index] * i[index];
	}

	return (uint32_t)(mac / (uint64_t)size);
}

void CalculatePower(void)
{
	uint32_t tmp;

	if(State == 0)
	{
		return;
	}

	__fir(voltage, WaveCoeff, NWaveCoeff, 2100);
	__fir(current, WaveCoeff, NWaveCoeff, 2100);

	if(PhaselossReq == 1)
	{
		tmp = CalculateStd(current, 2048);
		power.std_1 = tmp;
		power.std = tmp;
		PhaselossReq = 2;
	}

	power.bcurrent = CalculateRMS(current, 2048, 0);
	power.bvoltadge = CalculateRMS(voltage, 2048, 0);
	power.bpower = power.bcurrent * power.bvoltadge / 100;

	power.current = ((int64_t)sysCoeff.iCoeff.k * power.bcurrent + sysCoeff.iCoeff.b) >> 15;
	power.voltadge = ((int64_t)sysCoeff.uCoeff.k * power.bvoltadge + sysCoeff.uCoeff.b) >> 15;
	power.power = power.current * power.voltadge / 100;

	DMA1_Channel1->CCR &= ~1ul;
	DMA2_Channel5->CCR &= ~1ul;
	DMA1_Channel1->CNDTR = 2048 + 128;
	DMA2_Channel5->CNDTR = 2048 + 128;
	DMA1_Channel1->CMAR = (uint32_t)current;
	DMA2_Channel5->CMAR = (uint32_t)voltage;
	DMA1_Channel1->CCR |= 1;
	DMA2_Channel5->CCR |= 1;
	TIM2->CR1 = 0x05ul;

	State = 0;
}

void TIM2_IRQHandler(void)
{
	TIM2->SR = 0;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

void DMA1_Channel1_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_FLAG_TC1);
	TIM2->CR1 = 0;
	State = 1;
}
