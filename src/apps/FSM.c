#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fsm.h"
#include "GlobalFunc.h"

volatile uint32_t fsm = 1;

void fsm_set(uint32_t status)
{
	fsm = status;
}

uint32_t fsm_get(void)
{
	return fsm;
}

void ReadStatus(void *addr)
{
	SendChar(0x7b);
	SendChar(7);
	SendChar(0x00);
	SendChar(0x03);
	SendChar(fsm);

	SendChar(fsm + 0x19);
	SendChar(0x7d);
	delay();

	printf("%s\n", __func__);
}

void FSM_MainLoop(void)
{
	if(fsm == 9 && PhaselossReq == 2)
	{
		fsm = FSM_STATE_FINISH;
		PhaselossReq = 0;
	}
}
