#include "stm32f10x.h"
#include <stdio.h>
#include "GlobalFunc.h"
#include "datatype.h"

static void ReadWaveCallback(uint32_t ch, uint32_t origin, uint32_t step, uint32_t length);

extern uint32_t Ripple;

uint32_t SampleRate = 20000;
uint32_t SampleLength = 2048 + 128;
static volatile uint32_t SampleStatus = 0x0f;
CHANNEL_T WaveResult[6];
//POWER_T RippleResult;
extern POWER_T power;

uint16_t AdcBuff[8][2304];

void init_offset(void)
{
	StartSample();
	while(SampleStatus != 0x19);
	SampleStatus = 0x0f;
	WaveResult[0].center = CalculateMean(AdcBuff[0], 2048);
	WaveResult[1].center = CalculateMean(AdcBuff[1], 2048);
	WaveResult[2].center = CalculateMean(AdcBuff[2], 2048);
}

void CalculateSampleData(void)
{
	if(SampleStatus != 0)
	{
		return;
	}

	CalcSineWave(AdcBuff[0], &WaveResult[0]);
	CalcSineWave(AdcBuff[1], &WaveResult[1]);
	CalcSineWave(AdcBuff[2], &WaveResult[2]);
	CalcSquareWave(AdcBuff[3], &WaveResult[3]);
	CalcSquareWave(AdcBuff[4], &WaveResult[4]);
	CalcSquareWave(AdcBuff[5], &WaveResult[5]);

	CalculteHallResult();
	power.Ivcc = CalculateRMS(AdcBuff[6], SampleLength - 128, 0);
	power.Ivsp = CalculateRMS(AdcBuff[7], SampleLength - 128, 0);
	power.ripple = Ripple;
	fsm_set(9);

	SampleStatus = 0x0f;
}

void SetSampleRate(void *addr)
{
	SampleRate = *(uint32_t *)addr;
	SampleRate  = SampleRate >= 1000 ? SampleRate : 1000;
	SampleRate  = SampleRate <= 25000 ? SampleRate : 25000;
	SampleLength = 128 + *(uint32_t *)(addr + 4);
	SampleLength = SampleLength > 2176 ? 2176 : SampleLength;
	TIM1->ARR = 1000000 / SampleRate - 1;

	delay();
	SendACK();
	delay();

	printf("%s\n", __func__);
//	NVIC_SystemReset();
}

void ReadRippleResult(void *addr)
{
	uint8_t lrc = 0;

	SendChar(0x7b);
	lrc = sizeof(POWER_T) + 6;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x40);
	lrc += 0x40;

	lrc += SendBuffer((uint8_t *)&power, sizeof(POWER_T));

	SendChar(lrc);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}

void ReadWave(void *addr)
{
	uint32_t ch, origin, step, length;

	ch = *(uint8_t *)addr;
	addr++;
	origin = *(uint16_t *)addr;
	addr += 2;
	step = *(uint8_t *)addr;
	addr += 2;
	length = *(uint8_t *)addr;

	ReadWaveCallback(ch, origin, step, length);

	printf("%s\n", __func__);
}

void ReadResult(void *addr)
{
	uint32_t ch;
	uint8_t lrc = 0;

	ch = *(uint8_t *)addr;

	SendChar(0x7b);
	lrc = 52 + 7;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x11);
	lrc += 0x11;
	SendChar(ch);
	lrc += ch;

	lrc += SendBuffer((uint8_t *)&WaveResult[ch], sizeof(CHANNEL_T));

	SendChar(lrc);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}


void SampleFinish(void)
{
	if(SampleStatus == 1)
	{
		SampleStatus = 0x19;
	}
	else
	{
		SampleStatus = 0;
	}	
}

uint32_t getSampleStatus(void)
{
	return SampleStatus;
}

void setSampleStatus(uint32_t status)
{
	SampleStatus = status;
}

void StartTesting(void *addr)
{
	(void)addr;
	StartSample();
	StartHallCapture();
	fsm_set(FSM_STATE_RUN);
	PhaselossReq = 1;

	delay();
	delay();
	delay();
	delay();
	delay();
	SendACK();
	delay();

	printf("%s\n", __func__);
}

void StopTesting(void *addr)
{
	(void)addr;
	StopSample();
	StopHallCapture();
	fsm_set(FSM_STATE_IDLE);

	delay();
	SendACK();
	delay();

	printf("%s\n", __func__);
}

static void ReadWaveCallback(uint32_t ch, uint32_t origin, uint32_t step, uint32_t length)
{
	uint8_t lrc;

	SendChar(0x7b);
	lrc = length + 7;
	SendChar(lrc);
	SendChar(0x00);
	SendChar(0x10);
	lrc += 0x10;
	SendChar(ch);
	lrc += ch;

	for(int i = 0; i < length; i++)
	{
		uint32_t tmp;

		tmp = AdcBuff[ch][origin + i * step] >> 4;
		lrc += tmp;
		SendChar(tmp);
	}

	SendChar(lrc);
	SendChar(0x7d);
	delay();
}
