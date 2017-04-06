
#include <stdint.h>

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

typedef void( *pFunc )( void );

extern void SystemInit (void);            /* CMSIS System Initialization      */
extern void main(void);

void Default_Handler(void);                          /* Default empty handler */
void Reset_Handler(void);                            /* Reset Handler */

#ifndef __STACK_SIZE
  #define	__STACK_SIZE  0x00000400
#endif
static uint8_t stack[__STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

#ifndef __HEAP_SIZE
  #define	__HEAP_SIZE   0x00000200
#endif
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE]   __attribute__ ((aligned(8), used, section(".heap")));
#endif

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M3 Processor Exceptions */
void NMI_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler   (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler  (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));


/* ARMCM3 Specific Interrupts */
void WWDG_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TAMPER_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void RTCAlarm_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC3_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler")));
void FSMC_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void SDIO_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler  (void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler    (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Channel4_5_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
const pFunc __Vectors[] __attribute__ ((section(".vectors"))) = 
{ 	
	/* Cortex-M3 Exceptions Handler */
	(pFunc)&__StackTop,                       /*      Initial Stack Pointer     */
	Reset_Handler,                            /*      Reset Handler             */
	NMI_Handler,                              /*      NMI Handler               */
	HardFault_Handler,                        /*      Hard Fault Handler        */
	MemManage_Handler,                        /*      MPU Fault Handler         */
	BusFault_Handler,                         /*      Bus Fault Handler         */
	UsageFault_Handler,                       /*      Usage Fault Handler       */
	0,                                        /*      Reserved                  */
	0,                                        /*      Reserved                  */
	0,                                        /*      Reserved                  */
	0,                                        /*      Reserved                  */
	SVC_Handler,                              /*      SVCall Handler            */
	DebugMon_Handler,                         /*      Debug Monitor Handler     */
	0,                                        /*      Reserved                  */
	PendSV_Handler,                           /*      PendSV Handler            */
	SysTick_Handler,                          /*      SysTick Handler           */

	/* External interrupts */
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_IRQHandler,
	RTC_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_2_IRQHandler,
	USB_HP_CAN1_TX_IRQHandler,
	USB_LP_CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_IRQHandler,
	TIM1_UP_IRQHandler,
	TIM1_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTCAlarm_IRQHandler,
	USBWakeUp_IRQHandler,
	TIM8_BRK_IRQHandler,
	TIM8_UP_IRQHandler,
	TIM8_TRG_COM_IRQHandler,
	TIM8_CC_IRQHandler,
	ADC3_IRQHandler,
	FSMC_IRQHandler,
	SDIO_IRQHandler,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_5_IRQHandler	
};

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) 
{
	uint32_t *pSrc, *pDest;

/*  Copy data from read only memory to RAM.   */

	pSrc  = &__etext;
	pDest = &__data_start__;

	for ( ; pDest < &__data_end__ ; ) 
	{
		*pDest++ = *pSrc++;
	}

/*  Clear BSS sections.    */

	pDest = &__bss_start__;

	for ( ; pDest < &__bss_end__ ; ) 
	{
		*pDest++ = 0ul;
	}

	SystemInit();
	main();

}


/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void) 
{
	while(1);
}
