#include "stm32f10x.h"
#include <stdio.h>

extern void SendACK(void);

struct __vsp_t {
	int16_t dac1;
	int16_t dac2;
	uint16_t time1;
	uint16_t time2;
	int16_t cur1;
	int16_t cur2;
	int16_t step1;
	int16_t step2;
	uint8_t done1;
	uint8_t done2;
};

struct __vsp_t VspVcc;

void SetVccVsp(void *addr)
{
	StopPwm();
	DAC->DHR12RD = __ROR(*(uint32_t *)addr, 16);

	SendACK();

	printf("%s\n", __func__);
}

void SetVccVspEx(void *addr)
{
	uint32_t tmp;

	StopPwm();
	TIM6->CR1 = 0x00;
	tmp = *(uint32_t *)addr;
	VspVcc.dac1 = tmp & 0xffff;
	VspVcc.dac2 = tmp >> 16;
	VspVcc.dac1 = VspVcc.dac1 < 4096 ? VspVcc.dac1 : 4096;
	VspVcc.dac2 = VspVcc.dac2 < 4096 ? VspVcc.dac2 : 4096;
	addr += 4;
	tmp = *(uint32_t *)addr;
	VspVcc.time1 = tmp & 0xffff;
	VspVcc.time2 = tmp >> 16;
//	VspVcc.time1 /= 10;
//	VspVcc.time2 /= 10;
	VspVcc.time1 = VspVcc.time1 > 0 ? VspVcc.time1 : 1;
	VspVcc.time2 = VspVcc.time2 > 0 ? VspVcc.time2 : 1;

	VspVcc.cur1 = DAC->DOR1;
	VspVcc.cur1 = DAC->DOR1;

	VspVcc.step1 = (VspVcc.dac1 - VspVcc.cur1) / VspVcc.time1;
	VspVcc.step1 = 	VspVcc.step1 != 0 ? 	VspVcc.step1 : ((VspVcc.dac1 - VspVcc.cur1) > 0 ? 1 : -1);
	VspVcc.step2 = (VspVcc.dac2 - VspVcc.cur2) / VspVcc.time2;
	VspVcc.step2 = 	VspVcc.step2 != 0 ? 	VspVcc.step2 : ((VspVcc.dac2 - VspVcc.cur2) > 0 ? 1 : -1);

	if(VspVcc.dac1 - VspVcc.cur1 == 0)
	{
		VspVcc.done1 = 1;
	}
	else
	{
		VspVcc.done1 = 0;
	}
	if(VspVcc.dac2 - VspVcc.cur2 == 0)
	{
		VspVcc.done2 = 1;
	}
	else
	{
		VspVcc.done2 = 0;
	}

	TIM6->CR1 = 0x05;

	SendACK();

	printf("%s\n", __func__);
}

void TIM6_IRQHandler(void)
{
	TIM6->SR = 0;

	if(VspVcc.done1 == 0)
	{
		VspVcc.cur1 += VspVcc.step1;
		if(VspVcc.step1 > 0)
		{
			if(VspVcc.cur1 >= VspVcc.dac1)
			{
				VspVcc.cur1 = VspVcc.dac1;
				VspVcc.done1 = 1;
			}
		}
		else
		{
			if(VspVcc.cur1 <= VspVcc.dac1)
			{
				VspVcc.cur1 = VspVcc.dac1;
				VspVcc.done1 = 1;
			}
		}
		DAC->DHR12R1 = VspVcc.cur1;
	}

	if(VspVcc.done2 == 0)
	{
		VspVcc.cur2 += VspVcc.step2;
		if(VspVcc.step2 > 0)
		{
			if(VspVcc.cur2 >= VspVcc.dac2)
			{
				VspVcc.cur2 = VspVcc.dac2;
				VspVcc.done2 = 1;
			}
		}
		else
		{
			if(VspVcc.cur2 <= VspVcc.dac2)
			{
				VspVcc.cur2 = VspVcc.dac2;
				VspVcc.done2 = 1;
			}
		}
		DAC->DHR12R2 = VspVcc.cur2;
	}

	if(VspVcc.done2 && VspVcc.done1)
	{
		TIM6->CR1 = 0;
	}
}
