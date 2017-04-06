#include "stm32f10x.h"
#include "GlobalFunc.h"
#include "datatype.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

extern uint16_t AdcBuff[8][2304];
extern uint32_t SampleLength;
extern const int NWaveCoeff;
extern const int16_t WaveCoeff[];
extern CHANNEL_T WaveResult[];

HALL_T HallResult;
HALLCAP_T HallCap[3];
HALLFREQ_T HallFreq[3];
uint32_t Ripple;

void StartHallCapture(void)
{
	memset(HallCap, 0, sizeof(HallCap));
	memset(HallFreq, 0, sizeof(HallFreq));
	DWT->CYCCNT = 0;
	Ripple = 0;
	EXTI->RTSR = 0x600Bul;
	EXTI->FTSR = 0x6002ul;
	EXTI->PR = 0xfffful;
	EXTI->IMR = 0x600Bul;
	TIM5->CNT = 0;
	TIM5->CR1 = 0x0d;
	__DSB();
	__ISB();
	__NOP();
}

void StopHallCapture(void)
{
	EXTI->IMR = 0x0ul;
	EXTI->PR = 1 << 1 | 1 << 13 | 1 << 14;
	TIM5->CR1 = 0;
	__DSB();
	__NOP();
}

void ReadHallResult(void *addr)
{
	uint8_t lrc = 0;

	SendChar(0x7b);
	lrc = sizeof(HALL_T) + 6;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x13);
	lrc += 0x13;

	lrc += SendBuffer((uint8_t *)&HallResult, sizeof(HALL_T));

	SendChar(lrc);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}

void CalculateTime(HALLCAP_T *hallcap)
{
	int index;
	int32_t tmp;

	for(index = 0; index < (hallcap->ptr - 1); index++)
	{
		tmp = hallcap->Time[index + 1] - hallcap->Time[index];
		if(tmp < 0)
		{
			tmp = 0 - tmp;
		}
		hallcap->Time[index] = tmp;
	}

	for(; index < 15; index++)
	{
		hallcap->Time[index] = 0;
		hallcap->Level[index] = 0;
	}

	hallcap->ptr--;
	hallcap->ptr &= ~0x01ul;
}

void CalculateFreq(HALLCAP_T *hallcap, uint32_t *freq, uint32_t *duty)
{
	uint32_t Total = 0, High = 0;
	int index;

	for(index = 0; index < hallcap->ptr; index++)
	{
		Total += hallcap->Time[index];

		if(hallcap->Level[index] > 0)
		{
			High += hallcap->Time[index];
		}
	}

	if(Total > 0)
	{
		*duty = (High * 1000) / Total;
		*freq = (36000000000ull * index) / (uint64_t)Total;
	}
	else
	{
		*duty = 0;
		*freq = 0;
	}
}

uint32_t CalculatePhase(HALLCAP_T *hallcap)
{
	uint32_t index;

	for(index = 0; index < 15; index++)
	{
		if(hallcap->Level[index] > 0)
		{
			break;
		}
	}

	return hallcap->Time[index];
}

uint16_t CalculateCurrent(void)
{
	uint64_t mac = 0;
	uint16_t in;
	__fir(AdcBuff[6], WaveCoeff, NWaveCoeff, SampleLength);
	__fir(AdcBuff[7], WaveCoeff, NWaveCoeff, SampleLength);

	for(int index = 0; index < SampleLength - 128; index++)
	{
		in = AdcBuff[6][index];
		mac += in * in;
	}

	return (uint16_t)sqrt((double)mac / (double)(SampleLength - 128));
}

void CalculateHallFreq(HALLFREQ_T* hall, uint32_t* freq)
{
	if(hall->cnt > 3)
	{
		*freq = (uint32_t)((72000000000ull * (uint64_t)(hall->cnt - 1)) / (hall->eTime - hall->sTime));
	}
}

void CalculteHallResult(void)
{
	uint32_t APhase, BPhase, CPhase;
	int32_t tmp;

	APhase = CalculatePhase(&HallCap[0]);
	BPhase = CalculatePhase(&HallCap[1]);
	CPhase = CalculatePhase(&HallCap[2]);

	tmp = APhase - BPhase;
	HallResult.ABPhase = tmp >= 0 ? tmp : -tmp;
	tmp = APhase - CPhase;
	HallResult.ACPhase = tmp >= 0 ? tmp : -tmp;

	HallResult.result = 0;

	if(HallCap[0].ptr <= 3)
	{
		HallResult.ABPhase = 0;
		HallResult.ACPhase = 0;
		HallResult.ADuty = 0;
		HallResult.ADutyAve = 0;
		HallResult.AFreq = 0;
		HallResult.AFreqAve = 0;
	}
	else
	{
		HallResult.result |= 1;
		CalculateTime(&HallCap[0]);
		CalculateFreq(&HallCap[0], &HallResult.AFreqAve, &HallResult.ADutyAve);
		HallResult.ADuty = HallResult.ADutyAve;
		HallResult.AFreq = HallResult.AFreqAve;
	}

	if(HallCap[1].ptr <= 3)
	{
		HallResult.ABPhase = 0;
		HallResult.BDuty = 0;
		HallResult.BDutyAve = 0;
		HallResult.BFreq = 0;
		HallResult.BFreqAve = 0;
	}
	else
	{
		HallResult.result |= 1 << 1;
		HallResult.result |= 1 << 3;
		CalculateTime(&HallCap[1]);
		CalculateFreq(&HallCap[1], &HallResult.BFreqAve, &HallResult.BDutyAve);
		HallResult.BDuty = HallResult.BDutyAve;
		HallResult.BFreq = HallResult.BFreqAve;
	}

	if(HallCap[2].ptr <= 3)
	{
		HallResult.ACPhase = 0;
		HallResult.CDuty = 0;
		HallResult.CDutyAve = 0;
		HallResult.CFreq = 0;
		HallResult.CFreqAve = 0;
	}
	else
	{
		HallResult.result |= 1 << 2;
		HallResult.result |= 1 << 4;
		CalculateTime(&HallCap[2]);
		CalculateFreq(&HallCap[2], &HallResult.CFreqAve, &HallResult.CDutyAve);
		HallResult.CDuty = HallResult.CDutyAve;
		HallResult.CFreq = HallResult.CFreqAve;
	}

	if(HallResult.AFreqAve > 0)
	{
		HallResult.ABPhase = ((uint64_t)HallResult.ABPhase * 360000ull) / (72000000000ull / (uint64_t)HallResult.AFreqAve);
		HallResult.ACPhase = ((uint64_t)HallResult.ACPhase * 360000ull) / (72000000000ull / (uint64_t)HallResult.AFreqAve);;
	}

	HallResult.Current = CalculateCurrent();
	
	CalculateHallFreq(&HallFreq[0], &HallResult.AFreq);
	HallResult.AFreqAve = HallResult.AFreq;
	CalculateHallFreq(&HallFreq[1], &HallResult.BFreq);
	HallResult.BFreqAve = HallResult.BFreq;
	CalculateHallFreq(&HallFreq[2], &HallResult.CFreq);
	HallResult.CFreqAve = HallResult.CFreq;

	WaveResult[3].duty = HallResult.ADuty;
	WaveResult[3].freq = HallResult.AFreq;
	WaveResult[4].duty = HallResult.BDuty;
	WaveResult[4].freq = HallResult.BFreq;
	WaveResult[5].duty = HallResult.CDuty;
	WaveResult[5].freq = HallResult.CFreq;

	printf("%s\n", __func__);

}

void EXTI3_IRQHandler(void)
{
	EXTI->PR = 1 << 3;
	Ripple ++;
}

#define		HallCapIrq(ch, io, bit)										\
do{																		\
	uint32_t tmp = DWT->CYCCNT;											\
	EXTI->PR = 1 << bit;												\
	if(HallCap[ch].ptr < 15)											\
	{																	\
		HallCap[ch].Time[HallCap[ch].ptr] = tmp;						\
		HallCap[ch].Level[HallCap[ch].ptr] = ((io >> bit) & 0x01ul);	\
		HallCap[ch].ptr ++;												\
	}																	\
	if((io >> bit) & 0x01ul)											\
	{																	\
		if(HallFreq[ch].cnt == 0)										\
		{																\
			HallFreq[ch].sTime = tmp;									\
		}																\
		HallFreq[ch].cnt++;												\
		HallFreq[ch].eTime = tmp;										\
	}																	\
	__DSB();															\
}while(0)

void EXTI1_IRQHandler(void)
{
#if 0
	EXTI->PR = 1 << 1;
	HallCap[2].Time[HallCap[2].ptr] = DWT->CYCCNT;
	HallCap[2].Level[HallCap[2].ptr] = ((GPIOA->IDR >> 1) & 0x01ul);
	HallCap[2].ptr ++;
	if(HallCap[2].ptr >= 15)
	{
		EXTI->IMR &= ~(1 << 1);
	}
#endif
	HallCapIrq(2, GPIOA->IDR, 1);
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & 1 << 14)
	{
#if 0
		EXTI->PR = 1 << 14;
		HallCap[1].Time[HallCap[1].ptr] = DWT->CYCCNT;
		HallCap[1].Level[HallCap[1].ptr] = ((GPIOB->IDR >> 14) & 0x01ul);
		HallCap[1].ptr ++;
		if(HallCap[1].ptr >= 15)
		{
			EXTI->IMR &= ~(1 << 14);
		}
#endif
		HallCapIrq(1, GPIOB->IDR, 14);
	}

	if(EXTI->PR & 1 << 13)
	{
#if 0
		EXTI->PR = 1 << 13;
		HallCap[0].Time[HallCap[0].ptr] = DWT->CYCCNT;
		HallCap[0].Level[HallCap[0].ptr] = ((GPIOE->IDR >> 13) & 0x01ul);
		HallCap[0].ptr ++;
		if(HallCap[0].ptr >= 15)
		{
			EXTI->IMR &= ~(1 << 13);
		}
#endif
		HallCapIrq(0, GPIOE->IDR, 13);
	}
} 

void TIM5_IRQHandler(void)
{
	TIM5->CR1 = 0;
	TIM5->SR = 0;
	EXTI->IMR = 0;
	StopHallCapture();
	SampleFinish();
}


