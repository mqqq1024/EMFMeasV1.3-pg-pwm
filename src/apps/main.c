#include "stm32f10x.h"
#include "GlobalFunc.h"
#include <stdio.h>
#include <math.h>
#include "datatype.h"

extern void ScanMessage(void);
void CalculateSampleData(void);

int main(void)
{
	init_sys();
	init_offset();

	for(;;)
	{
		ScanMessage();
		CalculateSampleData();
		CalculatePower();
		FSM_MainLoop();
		delay();
	}
}

void delay(void)
{
	for(int i = 0; i < 0x4000; i++)
	{
		__NOP();
	}
}
