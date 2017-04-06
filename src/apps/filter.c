#include <stdint.h>
#include "stm32f10x.h"

void __fast_fir(int16_t *array, const int16_t *coeff, uint32_t size)
{
	int32_t conv;

	for(int i = 0; i < size; i++)
	{
		conv = 0;
		conv += coeff[19] * array[i];
		conv += coeff[18] * array[i + 1];
		conv += coeff[17] * array[i + 2];
		conv += coeff[16] * array[i + 3];
		conv += coeff[15] * array[i + 4];
		conv += coeff[14] * array[i + 5];
		conv += coeff[13] * array[i + 6];
		conv += coeff[12] * array[i + 7];
		conv += coeff[11] * array[i + 8];
		conv += coeff[10] * array[i + 9];
		conv += coeff[9] * array[i + 10];
		conv += coeff[8] * array[i + 11];
		conv += coeff[7] * array[i + 12];
		conv += coeff[6] * array[i + 13];
		conv += coeff[5] * array[i + 14];
		conv += coeff[4] * array[i + 15];
		conv += coeff[3] * array[i + 16];
		conv += coeff[2] * array[i + 17];
		conv += coeff[1] * array[i + 18];
		conv += coeff[0] * array[i + 19];

		conv /= 32768;
		array[i] = conv;
	}
}

void __fir(uint16_t *array, const int16_t *coeff, uint32_t N, uint32_t length)
{
	int32_t conv;
	int16_t tmp;

	for(int i = 0; i < length - N; i++)
	{
		conv = 0;
		for(int j = 0; j < N; j++)
		{
			conv += coeff[j] * array[i + j];
		}

		tmp = __SSAT(conv / 32768, 15);
		array[i] = tmp > 0 ? tmp : 0;
	}
}
