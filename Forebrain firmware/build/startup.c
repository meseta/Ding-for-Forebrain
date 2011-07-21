/******************************************************************************
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/

#include "LPC13xx.h"
#include "uafunc.h"
#include "config.h"

#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
#define WEAK __attribute__ ((weak))

extern void* __stack_top;
extern unsigned long _etext;
extern unsigned long __data;
extern unsigned long __edata;
extern unsigned long __bss;
extern unsigned long __ebss;

extern WEAK int main(void);
extern void SystemInit(void);

void GeneralFault(void) {
	volatile unsigned int i;
	LPC_GPIO3->DIR |= 0b1111;
	while(1) {
		for(i=0; i<1000000; i++);
		LPC_GPIO3->DATA = 0b1100;
		for(i=0; i<1000000; i++);
		LPC_GPIO3->DATA = 0b0011;
	}
}

void EmptyFunction(void) {
	return;
}

void I2C_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void TIMER16_0_IRQHandler(void) WEAK_ALIAS(EmptyFunction);
void TIMER16_1_IRQHandler(void) WEAK_ALIAS(EmptyFunction);
void TIMER32_0_IRQHandler(void) WEAK_ALIAS(EmptyFunction);
void TIMER32_1_IRQHandler(void) WEAK_ALIAS(EmptyFunction);
void SSP_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void UART_IRQHandler(void)      WEAK_ALIAS(EmptyFunction);
void USB_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void USB_FIQHandler(void)       WEAK_ALIAS(EmptyFunction);
void ADC_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void WDT_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void BOD_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void FMC_IRQHandler(void)       WEAK_ALIAS(EmptyFunction);
void PIOINT3_IRQHandler(void)   WEAK_ALIAS(EmptyFunction);
void PIOINT2_IRQHandler(void)   WEAK_ALIAS(EmptyFunction);
void PIOINT1_IRQHandler(void)   WEAK_ALIAS(EmptyFunction);
void PIOINT0_IRQHandler(void)   WEAK_ALIAS(EmptyFunction);
void WAKEUP_IRQHandler(void)    WEAK_ALIAS(EmptyFunction);
void SysTick_Handler(void)      WEAK_ALIAS(EmptyFunction);

void NMI_Handler(void)          WEAK_ALIAS(GeneralFault);
void HardFault_Handler(void)    WEAK_ALIAS(GeneralFault);
void MemManage_Handler(void)    WEAK_ALIAS(GeneralFault);
void BusFault_Handler(void)     WEAK_ALIAS(GeneralFault);
void UsageFault_Handler(void)   WEAK_ALIAS(GeneralFault);
void SVCall_Handler(void)       WEAK_ALIAS(GeneralFault);
void DebugMon_Handler(void)     WEAK_ALIAS(GeneralFault);
void PendSV_Handler(void)       WEAK_ALIAS(GeneralFault);

void Reset_Handler(void) {
	unsigned long *pSrc, *pDest;
	unsigned int i;

	// Copy the data segment initializers from flash to SRAM 
	pSrc = &_etext;
	for(pDest = &__data; pDest < &__edata; ) {
	  *pDest++ = *pSrc++;
	}

	// Zero fill the bss segment 
	for(pDest = &__bss; pDest < &__ebss; ) {
	  *pDest++ = 0;
	}
	
	// Clock setup
	LPC_SYSCON->PDRUNCFG &= ~(1 << 5);				// Power up system oscillator
	for (i = 0; i < 1000; i++);						// Brief delay
	LPC_SYSCON->SYSOSCCTRL = 0;						// System oscillator setup - not bypass, 1-20MHz range
	LPC_SYSCON->SYSPLLCLKSEL = 0x01;   				// Select system oscillator as PLL source
	LPC_SYSCON->SYSPLLCLKUEN = 0x00;               	// Update clock source
	LPC_SYSCON->SYSPLLCLKUEN = 0x01;
	while (!(LPC_SYSCON->SYSPLLCLKUEN & 0x01));     // Wait for update
	LPC_SYSCON->SYSPLLCTRL = 0x25;					// Select PLL divider to 6 (12MHz - 72MHz)
	LPC_SYSCON->PDRUNCFG &= ~(1 << 7);          	// Power up PLL
	while (!(LPC_SYSCON->SYSPLLSTAT & 0x01));	    // Wait for PLL lock
	LPC_SYSCON->MAINCLKSEL = 0x03;     				// Select PLL as main clock source
	LPC_SYSCON->MAINCLKUEN = 0x00;               	// Update clock source
	LPC_SYSCON->MAINCLKUEN = 0x01;
	while (!(LPC_SYSCON->MAINCLKUEN & 0x01));       // Wait for clock update
	
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	Port0Init(ALL & ~PIN0);					// Set all pins to digital IO (except Port 0, Pin 0, which is RESET)
	Port1Init(ALL);
	Port2Init(ALL);
	Port3Init(ALL);

	#if SYSTICK_EN
		SysTickInit();
	#endif
	
	main();
}

__attribute__ ((section(".vectors"), used))
const void *vectors[] =
{
	&__stack_top,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,
	SVCall_Handler,
	DebugMon_Handler, 
	0,
	PendSV_Handler,
	SysTick_Handler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	WAKEUP_IRQHandler,
	I2C_IRQHandler,
	TIMER16_0_IRQHandler,
	TIMER16_1_IRQHandler,
	TIMER32_0_IRQHandler,
	TIMER32_1_IRQHandler,
	SSP_IRQHandler,
	UART_IRQHandler,
	USB_IRQHandler,
	USB_FIQHandler,
	ADC_IRQHandler,
	WDT_IRQHandler,
	BOD_IRQHandler,
	FMC_IRQHandler,
	PIOINT3_IRQHandler,
	PIOINT2_IRQHandler,
	PIOINT1_IRQHandler,
	PIOINT0_IRQHandler,
};