#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>
#include "GlobalFunc.h"

typedef	struct	__pwm_t
{
	uint16_t	amplitude;
	uint16_t	duty;
	uint8_t		freq;
	uint8_t		cmd;
}PWM_T;

static uint8_t	pwm_status = 0;
PWM_T	pwm;

void StopPwm(void)
{
	TIM4->CR1 = 0x00;
	TIM4->CCER = 0;
	pwm_status = 0;
}


void SetPwm(void *addr)
{
	TIM6->CR1 = 0;
	memcpy(&pwm, addr, sizeof(pwm));

	if(pwm.cmd != 0x55)
	{
		TIM4->CR1 = 0x00;
		TIM4->CCER = 0;
		DAC->DHR12L1 = 0;
		pwm_status = 0;
	}
	else
	{
		switch(pwm.freq)
		{
		case 1:
 			TIM4->PSC = 1;
			TIM4->ARR = 36000-1;
			break;
		case 8:
 			TIM4->PSC = 0;
			TIM4->ARR = 9000-1;
			break;
		case 10:
 			TIM4->PSC = 0;
			TIM4->ARR = 7200-1;
			break;
		default:
 			TIM4->PSC = 0;
			TIM4->ARR = 3600-1;
			break;
		}

		TIM4->CCR1 = (((uint64_t)(TIM4->ARR) * pwm.duty) / 10000) - 1;;//((3600 * pwm.duty) / 1000) - 1;
		DAC->DHR12R1 = pwm.amplitude;

		if(pwm_status == 0)
		{
			TIM4->CCER = 3;
			TIM4->CNT = 0;
			TIM4->CR1 = 0x05;
			pwm_status = 1;
		}
	}

	SendACK();
	delay();
}
