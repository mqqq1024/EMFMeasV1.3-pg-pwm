#include "stm32f10x.h"
#include "GlobalFunc.h"
#include "datatype.h"
#include <math.h>

extern const int NWaveCoeff;
extern const int16_t WaveCoeff[];
extern uint32_t SampleLength;

void CalcSquareWave(uint16_t *wave, CHANNEL_T *ch)
{
	uint32_t tmp = 0;
	uint32_t cnt;
	uint16_t in;
	uint32_t max, min;
	uint32_t index;

	/* hamming low pass filter */
	//__fir(wave, WaveCoeff, NWaveCoeff, SampleLength);

	/* find min and max, calculate center */
	max = 0;
	min = 0xffff;
	cnt = SampleLength - 128;
	for(index = 0; index < cnt; index++)
	{
		in = wave[index];
		tmp += in;
		if(max < in)
		{
			max = in;
		}
		if(min > in)
		{
			min = in;
		}
	}

	ch->max = max;
	ch->min = min;
	ch->center = __USAT(tmp/cnt, 16);

	if(ch->center < 128 || ch->max - ch->min < 256)
	{
		ch->zeroNum = 0;
		ch->freq = 0;
		ch->duty = 0;
	}
	else
	{
		/* find zero cross */
		in = wave[0] > ch->center ? 1 : 0;
		ch->zeroNum = 0;
		for(index = 0; index < cnt; index++)
		{
			if(in)
			{
				if(wave[index] <= ch->center)
				{
					ch->zeroPos[ch->zeroNum] = index;
					ch->zeroNum ++;
					in = !in;
				}
			}
			else
			{
				if(wave[index] > ch->center)
				{
					ch->zeroPos[ch->zeroNum] = index;
					ch->zeroNum ++;
					in = !in;
				}
			}

			if(ch->zeroNum >= 15)
			{
				break;
			}
		}
	}

	/* calculate hlevel, llevel, freq, duty */
	if(ch->zeroNum >= 3)
	{
		tmp = 0;
		for(index = ch->zeroPos[0]; index < ch->zeroPos[1]; index++)
		{
			tmp += wave[index];
		}
		tmp /= (ch->zeroPos[1] - ch->zeroPos[0]);
		if(tmp > ch->center)
		{
			ch->hLevel = tmp;
			in = 0;
		}
		else
		{
			ch->lLevel = tmp;
		}

		tmp = 0;
		for(index = ch->zeroPos[1]; index < ch->zeroPos[2]; index++)
		{
			tmp += wave[index];
		}
		tmp /= (ch->zeroPos[2] - ch->zeroPos[1]);
		if(tmp > ch->center)
		{
			ch->hLevel = tmp;
			in = 1;
		}
		else
		{
			ch->lLevel = tmp;
		}
	}
	else
	{
		tmp = 0;
		for(index = 0; index < 1024; index++)
		{
			tmp += wave[index];
		}
		tmp /= 1024;
		ch->hLevel = tmp;
		ch->lLevel = tmp;
		ch->freq = 0;
		ch->duty = 0;
	}
}

void CalcSineWave(uint16_t *wave, CHANNEL_T *ch)
{
	uint32_t tmp = 0;
	uint32_t cnt;
	uint16_t in;
	uint32_t max, min;
	uint32_t index;

	/* hamming low pass filter */
	__fir(wave, WaveCoeff, NWaveCoeff, SampleLength);

	/* find min and max, calculate center */
	max = 0;
	min = 0xffff;
	cnt = SampleLength - 128;
	for(index = 0; index < cnt; index++)
	{
		in = wave[index];
		tmp += in;
		if(max < in)
		{
			max = in;
		}
		if(min > in)
		{
			min = in;
		}
	}

	ch->max = max;
	ch->min = min;

	if(ch->max - ch->min < 256)
	{
		ch->zeroNum = 0;
		ch->freq = 0;
		ch->duty = 0;
	}
	else
	{
		/* find zero cross */
		in = wave[0] > ch->center ? 1 : 0;
		ch->zeroNum = 0;
		for(index = 0; index < cnt; index++)
		{
			if(in)
			{
				if(wave[index] <= ch->center)
				{
					ch->zeroPos[ch->zeroNum] = index;
					ch->zeroNum ++;
					in = !in;
				}
			}
			else
			{
				if(wave[index] > ch->center)
				{
					ch->zeroPos[ch->zeroNum] = index;
					ch->zeroNum ++;
					in = !in;
				}
			}

			if(ch->zeroNum >= 15)
			{
				break;
			}
		}
	}

	/* calculate Vrms */
	if(ch->zeroNum >= 15)
	{
		tmp = CalculateRMS(&wave[ch->zeroPos[0]], ch->zeroPos[ch->zeroNum - 1] - ch->zeroPos[0], ch->center);
		ch->RmsVal = tmp;
		ch->RmsDat = tmp;
		ch->RmsHaff = tmp;
		ch->freq = 0;
		ch->duty = 0;
	}
	else
	{
		tmp = CalculateRMS(wave, 1024, ch->center);
		ch->RmsVal = tmp;
		ch->RmsDat = tmp;
		ch->RmsHaff = tmp;
		ch->freq = 0;
		ch->duty = 0;
	}
}


uint32_t CalculateMean(uint16_t *buf, uint32_t size)
{
	uint32_t sum = 0;

	for(int index = 0; index < size; index++)
	{
		sum += buf[index];
	}
	sum /= size;

	return sum;
}

uint32_t CalculateStd(uint16_t *buf, uint32_t size)
{
	uint32_t	mean;
	uint64_t	sum = 0;
	int tmp;
	mean = CalculateMean(buf, size);

	for(int i = 0; i < size; i++)
	{
		tmp = buf[i] - mean;
		tmp = tmp * tmp;
		sum += tmp;
	}

	mean = sum / size;

	return mean;
}


uint32_t CalculateRMS(uint16_t *buf, uint32_t size, uint16_t offset)
{
	uint64_t mac = 0;
	int32_t in;

	for(int index = 0; index < size; index++)
	{
		in = buf[index] - offset;
		mac += in * in;
	}

	return (uint32_t)sqrt((double)mac / (double)size);
}
