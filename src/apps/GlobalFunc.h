/*
 * GlobalFunc.h
 *
 *  Created on: 2015-9-15
 *      Author: Sattic.Kuuo
 */

#include "datatype.h"
#include <stdint.h>
#include "FSM.h"

#ifndef GLOBALFUNC_H_
#define GLOBALFUNC_H_

extern volatile uint32_t		PhaselossReq;

void init_sys(void);
void init_offset(void);
void delay(void);

void init_at45db041e(void);
void AT45DB041E_Read(uint32_t addr, uint8_t *buf, uint32_t size);
void AT45DB041E_Write(uint32_t addr, uint8_t *buf, uint32_t size);

int SendChar(int);
uint8_t SendBuffer(uint8_t *buf, uint32_t size);
void SendACK(void);
void __fast_fir(int16_t *scr, const int16_t *coeff, uint32_t N);
void __fir(uint16_t *array, const int16_t *coeff, uint32_t N, uint32_t length);

void SetVccVsp(void *addr);
void SetVccVspEx(void *addr);
void SetSampleRate(void *addr);
void StartTesting(void *addr);
void StopTesting(void *addr);
void ReadWave(void *addr);
void ReadResult(void *addr);
void ReadHallResult(void *addr);
void ReadPower(void *addr);
void WriteSysCoeff(void *addr);
void ReadSysCoeff(void *addr);
void ReadStatus(void *addr);
void RelayControl(void *addr);
void ReadRippleResult(void *addr);
void SetPwm(void *addr);

void StartSample(void);
void StopSample(void);
void SampleFinish(void);
void StopPwm(void);

void StartHallCapture(void);
void StopHallCapture(void);
void CalculteHallResult(void);

void setSampleStatus(uint32_t status);
uint32_t getSampleStatus(void);

void CalcSquareWave(uint16_t *wave, CHANNEL_T *ch);
void CalcSineWave(uint16_t *wave, CHANNEL_T *ch);
uint32_t CalculateRMS(uint16_t *buf, uint32_t size, uint16_t offset);
uint32_t CalculateMean(uint16_t *buf, uint32_t size);
uint32_t CalculateStd(uint16_t *buf, uint32_t size);

void CalculatePower(void);

void FSM_MainLoop(void);

#endif /* GLOBALFUNC_H_ */
