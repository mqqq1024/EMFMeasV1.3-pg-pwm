#include "stm32f10x.h"
#include <stdio.h>
#include "GlobalFunc.h"

static int32_t p1, p2;
extern uint16_t AdcBuff[8][2304];
extern uint32_t SampleLength;


void StartSample(void)
{
	p1 = 0;
	p2 = 0;
	TIM1->CR1 = 0x05ul;
	setSampleStatus(1);
}

void StopSample(void)
{
	TIM1->CR1= 0x04ul;
	setSampleStatus(0);
}

void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1, ADC_IT_JEOC))
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
		AdcBuff[0][p1] = ADC1->JDR1;
		AdcBuff[1][p1] = ADC1->JDR2;
		AdcBuff[2][p1] = ADC1->JDR3;
		AdcBuff[6][p1] = ADC1->JDR4;
		p1++;

		if(p1 >= SampleLength)
		{
			TIM1->CR1= 0x04ul;

			SampleFinish();
#if 0			
			StopHallCapture();
#endif
		}
	}

	if(ADC_GetITStatus(ADC2, ADC_IT_JEOC))
	{
		ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
		AdcBuff[3][p2] = ADC2->JDR1;
		AdcBuff[4][p2] = ADC2->JDR2;
		AdcBuff[5][p2] = ADC2->JDR3;
		AdcBuff[7][p2] = ADC2->JDR4;
		p2++;
	}
}

void ADC3_IRQHandler(void)
{
	ADC_ClearITPendingBit(ADC3, ADC_IT_JEOC);
}
