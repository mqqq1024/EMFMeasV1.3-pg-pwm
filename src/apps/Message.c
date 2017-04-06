#include <stdio.h>
#include "GlobalFunc.h"

#define 	START_TESTING		0x01
#define		STOP_TESTING		0x02
#define		SET_VCCVSP			0x23
#define		SET_SAMPLERATE		0x20

typedef struct __message_t{
	uint8_t Cmd;
	uint8_t Length;
	uint8_t *Addr;
} message_t;

static message_t Message = {0,0,0};

void Message_Post(uint8_t cmd, uint8_t *addr, uint32_t size)
{
	Message.Cmd = cmd;
	Message.Addr = addr;
	Message.Length = size;
}

void ScanMessage(void)
{
	switch(Message.Cmd)
	{
	case 0x25:
		Message.Cmd = 0;
		SendACK();
		break;
	case 0x23:
		Message.Cmd = 0;
		SetVccVsp(Message.Addr);
		break;
	case 0x22:
		Message.Cmd = 0;
		SetVccVspEx(Message.Addr);
		break;
	case 0x20:
		Message.Cmd = 0;
		SetSampleRate(Message.Addr);
		break;
	case 0x01:
		Message.Cmd = 0;
		StartTesting(Message.Addr);
		break;
	case 0x02:
		Message.Cmd = 0;
		StopTesting(Message.Addr);
		break;
	case 0x03:
		Message.Cmd = 0;
		ReadStatus(Message.Addr);
		break;
	case 0x10:
		Message.Cmd = 0;
		ReadWave(Message.Addr);
		break;
	case 0x11:
		Message.Cmd = 0;
		ReadResult(Message.Addr);
		break;
	case 0x13:
		Message.Cmd = 0;
		ReadHallResult(Message.Addr);
		break;
	case 0x29:
		Message.Cmd = 0;
		RelayControl(Message.Addr);
		break;
	case 0x2A:
		Message.Cmd = 0;
		SetPwm(Message.Addr);
		break;
	case 0x40:
		Message.Cmd = 0;
		ReadRippleResult(Message.Addr);
		break;
	case 0x41:
		Message.Cmd = 0;
		ReadPower(Message.Addr);
		break;
	case 0x48:
		Message.Cmd = 0;
		ReadSysCoeff(Message.Addr);
		break;
	case 0x49:
		Message.Cmd = 0;
		WriteSysCoeff(Message.Addr);
		break;
	}
}




