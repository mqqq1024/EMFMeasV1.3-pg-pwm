#include "stm32f10x.h"

void init_at45db041e(void);
void init_coeff(void);

#define 	ADC1_DR_Address    ((uint32_t)0x4001244C)
#define 	ADC3_DR_Address    ((uint32_t)0x40013C4C)
#define  DWT_CR      *(volatile u32 *)0xE0001000
#define  DWT_CYCCNT  *(volatile u32 *)0xE0001004
#define  DEM_CR      *(volatile u32 *)0xE000EDFC
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

extern uint16_t voltage[];
extern uint16_t current[];

static void init_clk(void)
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7 | RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
}

static void init_io(void)
{
	GPIOA->CRL = 0x00004040ul;
	GPIOA->CRH = 0x444344B3ul;
	GPIOB->CRL = 0x34444440ul;
	GPIOB->CRH = 0x44444443ul;
	GPIOC->CRL = 0x44000000ul;
	GPIOC->CRH = 0x44444444ul;
	GPIOD->CRL = 0x44444333ul;
	GPIOD->CRH = 0x444B4444ul;
	GPIOE->CRL = 0x44444444ul;
	GPIOE->CRH = 0x44444444ul;

	AFIO->MAPR = 1 << 12;
	AFIO->EXTICR[0] = 0x0000ul;
	AFIO->EXTICR[3] = 0x140ul;
}

static void init_adc(void)
{
	ADC_InitTypeDef   	ADC_InitStructure;

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
	ADC_DMACmd(ADC1, ENABLE);

	ADC_InjectedSequencerLengthConfig(ADC1, 4);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_55Cycles5);
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);
	ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_DeInit(ADC2);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);

	ADC_InjectedSequencerLengthConfig(ADC2, 4);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_14, 2, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_15, 3, ADC_SampleTime_55Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_10, 4, ADC_SampleTime_55Cycles5);
	ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_T1_CC4);
	ADC_ExternalTrigInjectedConvCmd(ADC2, ENABLE);
	ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
	ADC_ITConfig(ADC2, ADC_IT_JEOC, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));

	ADC_DeInit(ADC3);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC3, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);
	ADC_DMACmd(ADC3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
	ADC_ResetCalibration(ADC3);
	while(ADC_GetResetCalibrationStatus(ADC3));
	ADC_StartCalibration(ADC3);
	while(ADC_GetCalibrationStatus(ADC3));
}

static void init_tmr(void)
{

	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 49;
	TIM_TimeBaseStructure.TIM_Prescaler = 35;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, DISABLE);

	TIM2->CNT = 0;
	TIM2->PSC = 0;
	TIM2->ARR = 7199;
	TIM2->DIER = 1;
	TIM2->SR = 0;
	TIM2->CR2 = 0;
	TIM2->CR1 = 5;

	TIM4->CNT = 0;
	TIM4->PSC = 0;
	TIM4->ARR = 10000;//3599;
	TIM4->EGR = 1;
	TIM4->SR = 0;
	TIM4->CCMR1 = 0x60;
//	TIM4->CCER = 1;
//	TIM4->CCR1 = 999;
//	TIM4->CR2 = 0;
//	TIM4->CR1 = 5;

	TIM6->CNT = 0;
	TIM6->PSC = 71;
	TIM6->ARR = 9999;
	TIM6->DIER = 1;
	TIM6->SR = 0;
	TIM6->CR2 = 0;
	TIM6->CR1 = 4;

	TIM7->CNT = 0;
	TIM7->PSC = 71;
	TIM7->ARR = 9999;
	TIM7->EGR = 1;
	TIM7->SR = 0;
	TIM7->DIER = 1;
	TIM7->CR2 = 0;
	TIM7->CR1 = 0x00;
	
	TIM5->CNT = 0;
	TIM5->PSC = 7199;
	TIM5->ARR = 9999;
	TIM5->EGR = 1;
	TIM5->SR = 0;
	TIM5->DIER = 1;
}

static void init_dma(void)
{
	DMA_InitTypeDef    	DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)current;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2048 + 128;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);

	DMA_DeInit(DMA2_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)voltage;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2048 + 128;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel5, ENABLE);
}

static void init_uart(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

static void init_interrupt(void)
{
	NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_ClearPendingIRQ(TIM7_IRQn);
	NVIC_EnableIRQ(TIM7_IRQn);
	NVIC_ClearPendingIRQ(TIM6_IRQn);
	NVIC_EnableIRQ(TIM6_IRQn);
	NVIC_ClearPendingIRQ(TIM5_IRQn);
	NVIC_EnableIRQ(TIM5_IRQn);
	NVIC_ClearPendingIRQ(ADC1_2_IRQn);
	NVIC_EnableIRQ(ADC1_2_IRQn);
	NVIC_ClearPendingIRQ(ADC3_IRQn);
	NVIC_EnableIRQ(ADC3_IRQn);
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void init_dac(void)
{
	DAC->CR = 0x30003ul;
	DAC->DHR12RD = 0;
}

void init_sys(void)
{
	CoreDebug->DEMCR |= 1 << 24;
	DWT->CTRL |= 1;
	DWT->CYCCNT = 0;
	init_clk();
	init_io();
	init_at45db041e();
	init_coeff();
	init_dma();
	init_adc();
	init_tmr();
	init_uart();
	init_dac();
	init_interrupt();
}
