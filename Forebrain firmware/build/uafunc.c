/******************************************************************************
 * Version 1
 *
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/
 
#include "lpc13xx.h"
#include "uafunc.h"
#include "config.h"	// This contains all the configuration constants used here

/******************************************************************************
 * Misc Functions
 *****************************************************************************/
void Delay(unsigned int milliseconds) {
	#if SYSTICK
		SysTickDelayTicks = 0;
		while(SysTickDelayTicks < milliseconds/SYSTICK_MS);
	#else
		volatile unsigned int i, j;
		for(j=0; j<milliseconds; j++) {
			for(i=0; i<6000; i++);
		}
	#endif
}

#if IAP_EN
	IAP iap_entry = (IAP)0x1fff1ff1;
	
	void Reprogram(void) {
		IAPCommand[0] = 57;						// 57 is code to reinvoke ISP
		LPC_SYSCON->SYSAHBCLKDIV = 1;			// make sure clock divider is 1
		LPC_SYSCON->SYSAHBCLKCTRL |= 0x14440;	// turn on USB clock, Timer 32B1, GPIO, and IO blocks 

		__set_MSP(*((unsigned int *)0x1FFF0000));	// set pointer to bootloader ROM location
		iap_entry(IAPCommand, IAPResult);
	}
#endif

/******************************************************************************
 * Clock Functions
 *****************************************************************************/
// Clocks

#if SYSTICK_EN
	// ****** SysTick functions
	void SysTickInit(void) {	// this function is run automaticlly by the startup script if SysTick is enabled
		SysTick_Config(72000 * SYSTICK_MS); // this function is in core_cm3
		SysTicks = 0;
	}

	void SysTickStop(void) {
		SysTick->CTRL = 0x00;
	}
	
	void SysTick_Handler(void) {
		SysTicks++;
		SysTickDelayTicks++;
		Tick();
	}
	
	void SysTickDelay(unsigned int ticks) {
		SysTickDelayTicks = 0;
		while(SysTickDelayTicks < ticks);
	}
	
#endif

#if WDT_EN
	// ****** WDT functions
	void WDTInit(void) {
		// Set up watchdog clock
		LPC_SYSCON->WDTOSCCTRL = 0x020;		// Default rate of divider of 2, clock rate of 0.5MHz
		LPC_SYSCON->WDTCLKSEL = 0x02;		// Select the watchdog oscillator as input
		LPC_SYSCON->WDTCLKUEN = 0x01;		// Update clock source
		LPC_SYSCON->WDTCLKUEN = 0x00;	  
		LPC_SYSCON->WDTCLKUEN = 0x01;	  
		while (!(LPC_SYSCON->WDTCLKUEN & 0x01));     // Wait for update
		LPC_SYSCON->WDTCLKDIV = 1;			// Set divider to 1
		LPC_SYSCON->PDRUNCFG &= ~(1 << 6);	// Power up the WDT oscillator

		LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 15);		// Enable clock to WDT
		
		#if WDT_MODE
			// interrupt mode (WDT_MODE = 1)
			NVIC_EnableIRQ(WDT_IRQn);
			LPC_WDT->MOD = 0x1;
		#else
			// reset mode (WDT_MODE = 0)
			LPC_WDT->MOD = 0x3;
		#endif

		#if WDT_MS < 1
			LPC_WDT->TC = 1;
		#elif WDT_MS > 14400000
			LPC_WDT->TC = 14400000;
		#else
			LPC_WDT->TC = 83 * WDT_MS;		// WDT_MS is WDT timeout in MS
		#endif
		
		WDTFeed();
	}
#endif
/******************************************************************************
 * Power mode Functions
 *****************************************************************************/
 
/******************************************************************************
 * GPIO Functions
 *****************************************************************************/
// ****** Initialisation functions
void Port0Init(unsigned short pins) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->RESET_PIO0_0 = 0x50;
	if(pins & PIN1) LPC_IOCON->PIO0_1 = 0x50;
	if(pins & PIN2) LPC_IOCON->PIO0_2 = 0x50;
	if(pins & PIN3) LPC_IOCON->PIO0_3 = 0x50;
	if(pins & PIN4) LPC_IOCON->PIO0_4 = 0x00;
	if(pins & PIN5) LPC_IOCON->PIO0_5 = 0x00;
	if(pins & PIN6) LPC_IOCON->PIO0_6 = 0x50;
	if(pins & PIN7) LPC_IOCON->PIO0_7 = 0x50;
	if(pins & PIN8) LPC_IOCON->PIO0_8 = 0x50;
	if(pins & PIN9) LPC_IOCON->PIO0_9 = 0x50;
	if(pins & PIN10) LPC_IOCON->SWCLK_PIO0_10 = 0x51;
	if(pins & PIN11) LPC_IOCON->R_PIO0_11 = 0xD1;
	LPC_GPIO0->DATA = 0;
}

void Port1Init(unsigned short pins) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->R_PIO1_0 = 0xD1;
	if(pins & PIN1) LPC_IOCON->R_PIO1_1 = 0xD1;
	if(pins & PIN2) LPC_IOCON->R_PIO1_2 = 0xD1;
	if(pins & PIN3) LPC_IOCON->SWDIO_PIO1_3 = 0xD1;
	if(pins & PIN4) LPC_IOCON->PIO1_4 = 0xD0;
	if(pins & PIN5) LPC_IOCON->PIO1_5 = 0x50;
	if(pins & PIN6) LPC_IOCON->PIO1_6 = 0x50;
	if(pins & PIN7) LPC_IOCON->PIO1_7 = 0x50;
	if(pins & PIN8) LPC_IOCON->PIO1_8 = 0x50;
	if(pins & PIN9) LPC_IOCON->PIO1_9 = 0x50;
	if(pins & PIN10) LPC_IOCON->PIO1_10 = 0xD0;
	if(pins & PIN11) LPC_IOCON->PIO1_11 = 0xD0;
	LPC_GPIO1->DATA = 0;
}

void Port2Init(unsigned short pins) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO2_0 = 0x50;
	if(pins & PIN1) LPC_IOCON->PIO2_1 = 0x50;
	if(pins & PIN2) LPC_IOCON->PIO2_2 = 0x50;
	if(pins & PIN3) LPC_IOCON->PIO2_3 = 0x50;
	if(pins & PIN4) LPC_IOCON->PIO2_4 = 0x50;
	if(pins & PIN5) LPC_IOCON->PIO2_5 = 0x50;
	if(pins & PIN6) LPC_IOCON->PIO2_6 = 0x50;
	if(pins & PIN7) LPC_IOCON->PIO2_7 = 0x50;
	if(pins & PIN8) LPC_IOCON->PIO2_8 = 0x50;
	if(pins & PIN9) LPC_IOCON->PIO2_9 = 0x50;
	if(pins & PIN10) LPC_IOCON->PIO2_10 = 0x50;
	if(pins & PIN11) LPC_IOCON->PIO2_11 = 0x50;
	LPC_GPIO2->DATA = 0;
}

void Port3Init(unsigned short pins) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO3_0 = 0x50;
	if(pins & PIN1) LPC_IOCON->PIO3_1 = 0x50;
	if(pins & PIN2) LPC_IOCON->PIO3_2 = 0x50;
	if(pins & PIN3) LPC_IOCON->PIO3_3 = 0x50;
	LPC_GPIO3->DATA = 0;
}

// ****** Functions to set up Hysteresis
void Port0Hysteresis(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->RESET_PIO0_0 = (LPC_IOCON->RESET_PIO0_0 & 0xDF) | (value << 5);
	if(pins & PIN1) LPC_IOCON->PIO0_1 = (LPC_IOCON->PIO0_1 & 0xDF) | (value << 5);
	if(pins & PIN2) LPC_IOCON->PIO0_2 = (LPC_IOCON->PIO0_2 & 0xDF) | (value << 5);
	if(pins & PIN3) LPC_IOCON->PIO0_3 = (LPC_IOCON->PIO0_3 & 0xDF) | (value << 5);
	if(pins & PIN6) LPC_IOCON->PIO0_6 = (LPC_IOCON->PIO0_6 & 0xDF) | (value << 5);
	if(pins & PIN7) LPC_IOCON->PIO0_7 = (LPC_IOCON->PIO0_7 & 0xDF) | (value << 5);
	if(pins & PIN8) LPC_IOCON->PIO0_8 = (LPC_IOCON->PIO0_8 & 0xDF) | (value << 5);
	if(pins & PIN9) LPC_IOCON->PIO0_9 = (LPC_IOCON->PIO0_9 & 0xDF) | (value << 5);
	if(pins & PIN10) LPC_IOCON->SWCLK_PIO0_10 = (LPC_IOCON->SWCLK_PIO0_10 & 0xDF) | (value << 5);
	if(pins & PIN11) LPC_IOCON->R_PIO0_11 = (LPC_IOCON->R_PIO0_11 & 0xDF) | (value << 5);
}

void Port1Hysteresis(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->R_PIO1_0 = (LPC_IOCON->R_PIO1_0 & 0xDF) | (value << 5);
	if(pins & PIN1) LPC_IOCON->R_PIO1_1 = (LPC_IOCON->R_PIO1_1 & 0xDF) | (value << 5);
	if(pins & PIN2) LPC_IOCON->R_PIO1_2 = (LPC_IOCON->R_PIO1_2 & 0xDF) | (value << 5);
	if(pins & PIN3) LPC_IOCON->SWDIO_PIO1_3 = (LPC_IOCON->SWDIO_PIO1_3 & 0xDF) | (value << 5);
	if(pins & PIN4) LPC_IOCON->PIO1_4 = (LPC_IOCON->PIO1_4 & 0xDF) | (value << 5);
	if(pins & PIN5) LPC_IOCON->PIO1_5 = (LPC_IOCON->PIO1_5 & 0xDF) | (value << 5);
	if(pins & PIN6) LPC_IOCON->PIO1_6 = (LPC_IOCON->PIO1_6 & 0xDF) | (value << 5);
	if(pins & PIN7) LPC_IOCON->PIO1_7 = (LPC_IOCON->PIO1_7 & 0xDF) | (value << 5);
	if(pins & PIN8) LPC_IOCON->PIO1_8 = (LPC_IOCON->PIO1_8 & 0xDF) | (value << 5);
	if(pins & PIN9) LPC_IOCON->PIO1_9 = (LPC_IOCON->PIO1_9 & 0xDF) | (value << 5);
	if(pins & PIN10) LPC_IOCON->PIO1_10 = (LPC_IOCON->PIO1_10 & 0xDF) | (value << 5);
	if(pins & PIN11) LPC_IOCON->PIO1_11 = (LPC_IOCON->PIO1_11 & 0xDF) | (value << 5);
}

void Port2Hysteresis(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO2_0 = (LPC_IOCON->PIO2_0 & 0xDF) | (value << 5);
	if(pins & PIN1) LPC_IOCON->PIO2_1 = (LPC_IOCON->PIO2_1 & 0xDF) | (value << 5);
	if(pins & PIN2) LPC_IOCON->PIO2_2 = (LPC_IOCON->PIO2_2 & 0xDF) | (value << 5);
	if(pins & PIN3) LPC_IOCON->PIO2_3 = (LPC_IOCON->PIO2_3 & 0xDF) | (value << 5);
	if(pins & PIN4) LPC_IOCON->PIO2_4 = (LPC_IOCON->PIO2_4 & 0xDF) | (value << 5);
	if(pins & PIN5) LPC_IOCON->PIO2_5 = (LPC_IOCON->PIO2_5 & 0xDF) | (value << 5);
	if(pins & PIN6) LPC_IOCON->PIO2_6 = (LPC_IOCON->PIO2_6 & 0xDF) | (value << 5);
	if(pins & PIN7) LPC_IOCON->PIO2_7 = (LPC_IOCON->PIO2_7 & 0xDF) | (value << 5);
	if(pins & PIN8) LPC_IOCON->PIO2_8 = (LPC_IOCON->PIO2_8 & 0xDF) | (value << 5);
	if(pins & PIN9) LPC_IOCON->PIO2_9 = (LPC_IOCON->PIO2_9 & 0xDF) | (value << 5);
	if(pins & PIN10) LPC_IOCON->PIO2_10 = (LPC_IOCON->PIO2_10 & 0xDF) | (value << 5);
	if(pins & PIN11) LPC_IOCON->PIO2_11 = (LPC_IOCON->PIO2_11 & 0xDF) | (value << 5);
}

void Port3Hysteresis(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO3_0 = (LPC_IOCON->PIO3_0 & 0xDF) | (value << 5);
	if(pins & PIN1) LPC_IOCON->PIO3_1 = (LPC_IOCON->PIO3_1 & 0xDF) | (value << 5);
	if(pins & PIN2) LPC_IOCON->PIO3_2 = (LPC_IOCON->PIO3_2 & 0xDF) | (value << 5);
	if(pins & PIN3) LPC_IOCON->PIO3_3 = (LPC_IOCON->PIO3_3 & 0xDF) | (value << 5);
}

// ****** Functions to set up Pull-up/Pull-down resistors
void Port0Pull(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->RESET_PIO0_0 = (LPC_IOCON->RESET_PIO0_0 & 0xe7) | (value << 3);
	if(pins & PIN1) LPC_IOCON->PIO0_1 = (LPC_IOCON->PIO0_1 & 0xe7) | (value << 3);
	if(pins & PIN2) LPC_IOCON->PIO0_2 = (LPC_IOCON->PIO0_2 & 0xe7) | (value << 3);
	if(pins & PIN3) LPC_IOCON->PIO0_3 = (LPC_IOCON->PIO0_3 & 0xe7) | (value << 3);
	if(pins & PIN6) LPC_IOCON->PIO0_6 = (LPC_IOCON->PIO0_6 & 0xe7) | (value << 3);
	if(pins & PIN7) LPC_IOCON->PIO0_7 = (LPC_IOCON->PIO0_7 & 0xe7) | (value << 3);
	if(pins & PIN8) LPC_IOCON->PIO0_8 = (LPC_IOCON->PIO0_8 & 0xe7) | (value << 3);
	if(pins & PIN9) LPC_IOCON->PIO0_9 = (LPC_IOCON->PIO0_9 & 0xe7) | (value << 3);
	if(pins & PIN10) LPC_IOCON->SWCLK_PIO0_10 = (LPC_IOCON->SWCLK_PIO0_10 & 0xe7) | (value << 3);
	if(pins & PIN11) LPC_IOCON->R_PIO0_11 = (LPC_IOCON->R_PIO0_11 & 0xe7) | (value << 3);
}

void Port1Pull(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->R_PIO1_0 = (LPC_IOCON->R_PIO1_0 & 0xe7) | (value << 3);
	if(pins & PIN1) LPC_IOCON->R_PIO1_1 = (LPC_IOCON->R_PIO1_1 & 0xe7) | (value << 3);
	if(pins & PIN2) LPC_IOCON->R_PIO1_2 = (LPC_IOCON->R_PIO1_2 & 0xe7) | (value << 3);
	if(pins & PIN3) LPC_IOCON->SWDIO_PIO1_3 = (LPC_IOCON->SWDIO_PIO1_3 & 0xe7) | (value << 3);
	if(pins & PIN4) LPC_IOCON->PIO1_4 = (LPC_IOCON->PIO1_4 & 0xe7) | (value << 3);
	if(pins & PIN5) LPC_IOCON->PIO1_5 = (LPC_IOCON->PIO1_5 & 0xe7) | (value << 3);
	if(pins & PIN6) LPC_IOCON->PIO1_6 = (LPC_IOCON->PIO1_6 & 0xe7) | (value << 3);
	if(pins & PIN7) LPC_IOCON->PIO1_7 = (LPC_IOCON->PIO1_7 & 0xe7) | (value << 3);
	if(pins & PIN8) LPC_IOCON->PIO1_8 = (LPC_IOCON->PIO1_8 & 0xe7) | (value << 3);
	if(pins & PIN9) LPC_IOCON->PIO1_9 = (LPC_IOCON->PIO1_9 & 0xe7) | (value << 3);
	if(pins & PIN10) LPC_IOCON->PIO1_10 = (LPC_IOCON->PIO1_10 & 0xe7) | (value << 3);
	if(pins & PIN11) LPC_IOCON->PIO1_11 = (LPC_IOCON->PIO1_11 & 0xe7) | (value << 3);
}

void Port2Pull(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO2_0 = (LPC_IOCON->PIO2_0 & 0xe7) | (value << 3);
	if(pins & PIN1) LPC_IOCON->PIO2_1 = (LPC_IOCON->PIO2_1 & 0xe7) | (value << 3);
	if(pins & PIN2) LPC_IOCON->PIO2_2 = (LPC_IOCON->PIO2_2 & 0xe7) | (value << 3);
	if(pins & PIN3) LPC_IOCON->PIO2_3 = (LPC_IOCON->PIO2_3 & 0xe7) | (value << 3);
	if(pins & PIN4) LPC_IOCON->PIO2_4 = (LPC_IOCON->PIO2_4 & 0xe7) | (value << 3);
	if(pins & PIN5) LPC_IOCON->PIO2_5 = (LPC_IOCON->PIO2_5 & 0xe7) | (value << 3);
	if(pins & PIN6) LPC_IOCON->PIO2_6 = (LPC_IOCON->PIO2_6 & 0xe7) | (value << 3);
	if(pins & PIN7) LPC_IOCON->PIO2_7 = (LPC_IOCON->PIO2_7 & 0xe7) | (value << 3);
	if(pins & PIN8) LPC_IOCON->PIO2_8 = (LPC_IOCON->PIO2_8 & 0xe7) | (value << 3);
	if(pins & PIN9) LPC_IOCON->PIO2_9 = (LPC_IOCON->PIO2_9 & 0xe7) | (value << 3);
	if(pins & PIN10) LPC_IOCON->PIO2_10 = (LPC_IOCON->PIO2_10 & 0xe7) | (value << 3);
	if(pins & PIN11) LPC_IOCON->PIO2_11 = (LPC_IOCON->PIO2_11 & 0xe7) | (value << 3);
}

void Port3Pull(unsigned short pins, unsigned char value) {
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable clock to IOCON block
	if(pins & PIN0) LPC_IOCON->PIO3_0 = (LPC_IOCON->PIO3_0 & 0xe7) | (value << 3);
	if(pins & PIN1) LPC_IOCON->PIO3_1 = (LPC_IOCON->PIO3_1 & 0xe7) | (value << 3);
	if(pins & PIN2) LPC_IOCON->PIO3_2 = (LPC_IOCON->PIO3_2 & 0xe7) | (value << 3);
	if(pins & PIN3) LPC_IOCON->PIO3_3 = (LPC_IOCON->PIO3_3 & 0xe7) | (value << 3);
}


/******************************************************************************
 * USB HID functions
 ******************************************************************************/
#if USB_EN
	ROM ** rom = (ROM **)0x1fff1ff8;

	// ****** Information constants
	// these can be changed, however ensure the size of the array does not change
	const char USBstringInfo[] = {
		0x04, 3, 0x08, 0x09,					// Language Code (0x0809 is UK English)
		0x1c, 3, 'N',0,'X',0,'P',0,' ',0,'S',0,'E',0,'M',0,'I',0,'C',0,'O',0,'N',0,'D',0,' ',0,	// Manufacturer String
		0x28, 3, 'N',0,'X',0,'P',0,' ',0, 'L',0,'P',0,'C',0,'1',0,'3',0,'X',0,'X',0,' ',0,'H',0,'I',0,'D',0, ' ',0,' ',0, ' ',0, ' ',0,	// Product String
		0x1a, 3, 'F',0,'B',0,'R',0,'H',0,'I',0,'D',0,'0',0,'O',0,'O',0,	// Serial Number String
		0x0e, 3, 'H',0,'I',0,'D',0,' ',0,' ',0,' ',0,	// Interface 0, Setting 0
	};

	const HID_DEV_INFO USBhidInfo = {
		USB_VENDOR_ID,							// Vendor ID
		USB_PRODUCT_ID,							// Product ID
		USB_DEVICE_ID, 							// Device ID
		(uint32_t)&USBstringInfo,					// Description string pointer
		USB_IN_BYTES,							// Bytes for in report (up to 64)
		USB_OUT_BYTES,							// Bytes for out report (up to 64)
		USB_SAMPLE,								// Sample interval (in ms - up to 255, although it looks like 33ms is max)
		(uint32_t)&USBIn,						// In Report function pointer
		(uint32_t)&USBOut,						// Out Report function pointer
	};

	const USB_DEV_INFO USBdevInfo = {
		0x03, (uint32_t)&USBhidInfo,				// HID Class
	};

	// ****** Initialisation function
	void USBInit(void) {
		volatile unsigned int i;
		
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<14) | (1<<10) | (1<<16);	// Enable clocks for USB_REG, Timer32B1, IOCON
		
		(*rom)->pUSBD->init_clk_pins();   		// Initialise USB clock and pins (ROM function)
		for(i=0; i<1000; i++);					// Brief delay for clock to stabilise
		(*rom)->pUSBD->init(&USBdevInfo);			// Initialise USB peripheral (ROM function)

		// Bit of hax here, the ROM's built-in initialisation code sets the
		// main PLL to output the 48MHz required by USB, which leaves the
		// rest of the system running at 48MHz instead of the 72MHz we want.
		// So here we initialise the USB PLL to produce the 48MHz for the USB
		// peripheral (THAT'S WHAT IT WAS THERE FOR!), and reset the main PLL
		// back to 72MHz.
		
		LPC_SYSCON->USBPLLCLKSEL  = 0x01;   			// Select system oscillator as PLL source
		LPC_SYSCON->USBPLLCLKUEN  = 0x00;               // Update clock source
		LPC_SYSCON->USBPLLCLKUEN  = 0x01;
		while (!(LPC_SYSCON->USBPLLCLKUEN & 0x01));     // Wait for update
		LPC_SYSCON->USBPLLCTRL    = 0x03;				// Select PLL divider to 4 (12MHz - 48MHz)
		while (!(LPC_SYSCON->USBPLLSTAT & 0x01));		// Wait for PLL lock
		LPC_SYSCON->USBCLKSEL     = 0x00;               // Select PLL as main clock source
		LPC_SYSCON->USBCLKUEN = 0x00;               	// Update clock source
		LPC_SYSCON->USBCLKUEN = 0x01;
		while (!(LPC_SYSCON->USBCLKUEN & 0x01));       	// Wait for clock update
		
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

		USBConnect();							// Connect USB(ROM function)
		for(i=0; i<1000; i++);					// Brief wait before continuing to user code

		
		NVIC_SetPriority(USB_IRQn, 2);			// USB interrupt is lowered slightly in priority since it conflicts with serial data transfer like I2C, which is not very tolerant to being interrupted
		NVIC_SetPriority(USB_FIQn, 2);
		NVIC_SetPriority(TIMER_32_1_IRQn, 2);
	}

	void USBStop(void) {
		USBDisconnect();						// Disconnect USB
		NVIC_DisableIRQ(USB_IRQn);				// Disable interrupts
		NVIC_DisableIRQ(USB_FIQn);
		LPC_SYSCON->SYSAHBCLKCTRL &= ~((1<<14) | (1<<10));	// Disable clocks for USB and Timer32B1
	}

	void USB_IRQHandler(void) {
		(*rom)->pUSBD->isr();					// USB ISR (ROM function)
	}

	void USBConnect(void) {
		(*rom)->pUSBD->connect(1);
	}

	void USBDisconnect(void) {
		(*rom)->pUSBD->connect(0);
	}
#endif
/******************************************************************************
 * UART Functions
 *****************************************************************************/

/******************************************************************************
 * I2C Functions
 *****************************************************************************/
#if TIMER_EN
	// Mode: 0=timer, 1=counter, 2=hardware PWM
	// void T16B0Init(unsigned char mode, unsigned short value);
	// void T32B0Init(unsigned char mode, unsigned short value);
	// void T16B1Init(unsigned char mode, unsigned short value);
	#if !SPWM_EN
		//void T16B1Init(unsigned char mode, unsigned short value) {}
	#endif
#endif

#if I2C_EN
	// ******* Initialisation, set speed (in kHz)
	void I2CInit(unsigned short speed, unsigned char mode) {

		I2CMasterState=I2C_IDLE; I2CSlaveState=I2C_IDLE;
		I2CReadLength=0; I2CWriteLength=0;
		I2CReadIndex=0; I2CWriteIndex=0;


		LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 5);	// Enable clock to I2C

		LPC_IOCON->PIO0_4 = 0x01;	// Set up pins PIO0_4 and PIO0_5 for I2C
		LPC_IOCON->PIO0_5 = 0x01;

		if(speed > 400) {
			// if speed is greater than 400k, set to Fast-mode Plus
			LPC_IOCON->PIO0_4 |= (1 << 9);
			LPC_IOCON->PIO0_5 |= (1 << 9);
		}
		
		LPC_SYSCON->PRESETCTRL |= (1 << 1);	// I2C reset de-asserted
		LPC_I2C->CONCLR = I2C_AA | I2C_SI | I2C_STA | I2C_ENA;	// Clear status fags

		LPC_I2C->SCLL = (36000 / speed) & 0xffff;	// Set speed
		LPC_I2C->SCLH = (36000 / speed) & 0xffff;

		if(mode) {
			// set slave mode (I2C_MODE = 1)
			LPC_I2C->ADR0 = I2C_SLAVE_ADR0;	// slave mode addresses
			LPC_I2C->ADR1 = I2C_SLAVE_ADR1;
			LPC_I2C->ADR2 = I2C_SLAVE_ADR2;
			LPC_I2C->ADR3 = I2C_SLAVE_ADR3;
			LPC_I2C->CONSET = I2C_AA;
		}

		NVIC_EnableIRQ(I2C_IRQn);
		LPC_I2C->CONSET = I2C_ENA;
	}

	void I2CStop(void) {
		NVIC_DisableIRQ(I2C_IRQn);				// disable interrupt
		LPC_SYSCON->PRESETCTRL &= ~(1 << 1);	// I2C reset asserted
		LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 5);	// Disable clock to I2C
		Port0Init(PIN4 | PIN5);					// Set pins 4 and 5 to GPIO
	}

	// ****** The I2C Engine, does the I2C stuff
	void I2CEngine(void)  {
		unsigned int timeout = 0;
		I2CMasterState = I2C_IDLE;
		I2CMasterState2 = I2C_IDLE;
		I2CReadIndex = 0;
		I2CWriteIndex = 0;
		
		LPC_I2C->CONSET = I2C_STA;	// set start condition
		while(1) {
			// loop until start condition transmit detected or timeout and send stop
			if (I2CMasterState == I2C_STARTED) {
				while (1) {
					// once start state is transmitted, loop until NACK state then send stop
					if (I2CMasterState2 == I2C_NACK){
						LPC_I2C->CONSET = I2C_STO;	// set stop condition
						LPC_I2C->CONCLR = I2C_SI;	// clear interrupt flag

						while(LPC_I2C->CONSET & I2C_STO);	// wait until a stop condition
						break;
					}
				}
				break;	
			}
			if (timeout > I2C_TIMEOUT) {
				// timeout, send stop
				LPC_I2C->CONSET = I2C_STO;	// set stop condition
				LPC_I2C->CONCLR = I2C_SI;	// clear interrupt flag

				while(LPC_I2C->CONSET & I2C_STO);	// wait until a stop condition
				break;
			}
			timeout++;
		}
	}
	 
	// ****** Interrupt handler - I2C state is implemented using interrupts
	void I2C_IRQHandler(void) {
	
		switch (LPC_I2C->STAT & 0xff) {
			case 0x08:	// A START condition has been transmitted
				I2CWriteIndex = 0;
				LPC_I2C->DAT = I2CWriteData[I2CWriteIndex++];
				LPC_I2C->CONCLR = I2C_STA;
				I2CMasterState = I2C_STARTED;
				break;

			case 0x10:	// A Repeated START condition has been transmitted
					I2CReadIndex = 0;
					LPC_I2C->DAT = I2CWriteData[I2CWriteIndex++];
					LPC_I2C->CONCLR = I2C_STA;
					I2CMasterState = I2C_RESTARTED;
				break;

			case 0x18:	// SLA+W has been transmitted; ACK has been received
				if ( I2CMasterState == I2C_STARTED ) {
					LPC_I2C->DAT = I2CWriteData[I2CWriteIndex++];
					I2CMasterState = I2C_ACK;
				}
				break;

			case 0x20:	// SLA+W has not been transmitted; NOT ACK has been received
				I2CMasterState = I2C_NACK;
				break;
				
			case 0x28:	// Data byte in I2DAT has been transmitted; ACK has been received
				if ( I2CWriteIndex < I2CWriteLength ) {   
					LPC_I2C->DAT = I2CWriteData[I2CWriteIndex++];
					I2CMasterState = I2C_ACK;
				}
				else {
					if ( I2CReadLength > 0 ) {
						LPC_I2C->CONSET = I2C_STA;
						I2CMasterState = I2C_REPEATED_START;
					}
					else {
						I2CMasterState = I2C_NACK;
						I2CMasterState2 = I2C_NACK;	// hax is needed maybe?
						LPC_I2C->CONSET = I2C_STO;
					}
				}
				break;
				
			case 0x30:	// Data byte in I2DAT has been transmitted; NOT ACK has been received
				I2CMasterState = I2C_NACK;
				I2CMasterState2 = I2C_NACK;	// hax is needed maybe?
				LPC_I2C->CONSET = I2C_STO;
				break;
				
			case 0x38:	// Arbitration lost
				I2CMasterState = I2C_ERROR;
				break;

			case 0x40:	// SLA+R has been trnasmitted; ACK has been received
				if (I2CReadLength == 1) {
					LPC_I2C->CONCLR = I2C_AA;
				}
				else {
					LPC_I2C->CONSET = I2C_AA;
				}
				break;
				
			case 0x48:	// SLA+R has not been transmitted; NOT ACK has been received
				I2CMasterState = I2C_NACK;
				break;
				
			case 0x50:	// Data byte has been receievd; ACK has been returned
				I2CReadData[I2CReadIndex++] = LPC_I2C->DAT;
				if ( I2CReadIndex < I2CReadLength ) {   
					I2CMasterState = I2C_ACK;
					LPC_I2C->CONSET = I2C_AA;
				}
				else {
					I2CMasterState = I2C_NACK;
					LPC_I2C->CONCLR = I2C_AA;
				}
				break;

			case 0x58:	// Data byte has been received; NOT ACK has been returned
				I2CReadData[I2CReadIndex++] = LPC_I2C->DAT;
				I2CMasterState = I2C_NACK;
				I2CMasterState2 = I2C_NACK;	// hax is needed!
				LPC_I2C->CONSET = I2C_STO;
				break;
			
			// TODO: Slave mode
			
			default:	
				break;
		}

		LPC_I2C->CONCLR = I2C_SI;	// clear interrupt flag
	}

	// functions specifically for reading and writing a byte to Forebrain's EEPROM
	void EEPROMWrite(unsigned short address, unsigned char data) {
		I2CWriteLength = 4;
		I2CReadLength = 0;
		I2CWriteData[0] = I2C_EEPROM_ADR;
		I2CWriteData[1] = (address >> 8) & 0xff;	// ADDR high
		I2CWriteData[2] = address & 0xff;	// ADDR low
		I2CWriteData[3] = data;	// Data
		I2CEngine();
	}

	unsigned char EEPROMRead(unsigned short address) {
		I2CWriteLength = 3;
		I2CReadLength = 1;
		I2CWriteData[0] = I2C_EEPROM_ADR;
		I2CWriteData[1] = (address >> 8) & 0xff;	// ADDR high
		I2CWriteData[2] = address & 0xff;	// ADDR low
		I2CWriteData[3] = I2C_EEPROM_ADR | 1;
		I2CEngine();
		return I2CReadData[0];
	}
	
	void EEPROMBulkWrite(unsigned short address, unsigned char *data, unsigned int length) {
		unsigned int i;
		I2CWriteLength = 3 + length;
		I2CReadLength = 0;
		I2CWriteData[0] = I2C_EEPROM_ADR;
		I2CWriteData[1] = (address >> 8) & 0xff;	// ADDR high
		I2CWriteData[2] = address & 0xff;	// ADDR low
		for(i=0; (i<length && i<I2C_DATA_SIZE-3); i++) {
			I2CWriteData[3+i] = data[i];	// Data
		}
		I2CEngine();
	}

	void EEPROMBulkRead(unsigned short address, unsigned char *data, unsigned int length) {
		unsigned int i;
		I2CWriteLength = 3;
		I2CReadLength = length;
		I2CWriteData[0] = I2C_EEPROM_ADR;
		I2CWriteData[1] = (address >> 8) & 0xff;	// ADDR high
		I2CWriteData[2] = address & 0xff;	// ADDR low
		I2CWriteData[3] = I2C_EEPROM_ADR | 1;
		I2CEngine();
		
		for(i=0; (i<length && i<I2C_DATA_SIZE); i++) {
			data[i] = I2CReadData[i];
		}
	}
	
	
#endif
	
/******************************************************************************
 * SSP/SPI Functions
 *****************************************************************************/

/******************************************************************************
 * Timer Functions
 *****************************************************************************/
 #if SPWM_EN
	// Soft PWM: 10-bit PWM at 50Hz
	
	void SPWMInit(void) {
		unsigned char i;
		for(i=0; i<40; i++) SPWMCycle[i] = 1023;
		SPWMCount = 0;
		
		SPWMPort0 = 0;
		SPWMPort1 = 0;
		SPWMPort2 = 0;
		SPWMPort3 = 0;
		
		#ifdef SPWM_0_0
			SPWMPort0 |= 1 << 0;
		#endif
		#ifdef SPWM_0_1
			SPWMPort0 |= 1 << 1;
		#endif
		#ifdef SPWM_0_2
			SPWMPort0 |= 1 << 2;
		#endif
		#ifdef SPWM_0_3
			SPWMPort0 |= 1 << 3;
		#endif
		#ifdef SPWM_0_4
			SPWMPort0 |= 1 << 4;
		#endif
		#ifdef SPWM_0_5
			SPWMPort0 |= 1 << 5;
		#endif
		#ifdef SPWM_0_6
			SPWMPort0 |= 1 << 6;
		#endif
		#ifdef SPWM_0_7
			SPWMPort0 |= 1 << 7;
		#endif
		#ifdef SPWM_0_8
			SPWMPort0 |= 1 << 8;
		#endif
		#ifdef SPWM_0_9
			SPWMPort0 |= 1 << 9;
		#endif
		#ifdef SPWM_0_10
			SPWMPort0 |= 1 << 10;
		#endif
		#ifdef SPWM_0_11
			SPWMPort0 |= 1 << 11;
		#endif
		#ifdef SPWM_1_0
			SPWMPort1 |= 1 << 0;
		#endif
		#ifdef SPWM_1_1
			SPWMPort1 |= 1 << 1;
		#endif
		#ifdef SPWM_1_2
			SPWMPort1 |= 1 << 2;
		#endif
		#ifdef SPWM_1_3
			SPWMPort1 |= 1 << 3;
		#endif
		#ifdef SPWM_1_4
			SPWMPort1 |= 1 << 4;
		#endif
		#ifdef SPWM_1_5
			SPWMPort1 |= 1 << 5;
		#endif
		#ifdef SPWM_1_6
			SPWMPort1 |= 1 << 6;
		#endif
		#ifdef SPWM_1_7
			SPWMPort1 |= 1 << 7;
		#endif
		#ifdef SPWM_1_8
			SPWMPort1 |= 1 << 8;
		#endif
		#ifdef SPWM_1_9
			SPWMPort1 |= 1 << 9;
		#endif
		#ifdef SPWM_1_10
			SPWMPort1 |= 1 << 10;
		#endif
		#ifdef SPWM_1_11
			SPWMPort1 |= 1 << 11;
		#endif
		#ifdef SPWM_2_0
			SPWMPort2 |= 1 << 0;
		#endif
		#ifdef SPWM_2_1
			SPWMPort2 |= 1 << 1;
		#endif
		#ifdef SPWM_2_2
			SPWMPort2 |= 1 << 2;
		#endif
		#ifdef SPWM_2_3
			SPWMPort2 |= 1 << 3;
		#endif
		#ifdef SPWM_2_4
			SPWMPort2 |= 1 << 4;
		#endif
		#ifdef SPWM_2_5
			SPWMPort2 |= 1 << 5;
		#endif
		#ifdef SPWM_2_6
			SPWMPort2 |= 1 << 6;
		#endif
		#ifdef SPWM_2_7
			SPWMPort2 |= 1 << 7;
		#endif
		#ifdef SPWM_2_8
			SPWMPort2 |= 1 << 8;
		#endif
		#ifdef SPWM_2_9
			SPWMPort2 |= 1 << 9;
		#endif
		#ifdef SPWM_2_10
			SPWMPort2 |= 1 << 10;
		#endif
		#ifdef SPWM_2_11
			SPWMPort2 |= 1 << 11;
		#endif
		#ifdef SPWM_3_0
			SPWMPort3 |= 1 << 0;
		#endif
		#ifdef SPWM_3_1
			SPWMPort3 |= 1 << 1;
		#endif
		#ifdef SPWM_3_2
			SPWMPort3 |= 1 << 2;
		#endif
		#ifdef SPWM_3_3
			SPWMPort3 |= 1 << 3;
		#endif
		
		if(SPWMPort0) {
			Port0Init(SPWMPort0);
			Port0SetOut(SPWMPort0);;
		}
		if(SPWMPort1) {
			Port1Init(SPWMPort1);
			Port1SetOut(SPWMPort1);;
		}
		if(SPWMPort2) {
			Port2Init(SPWMPort2);
			Port2SetOut(SPWMPort2);;
		}
		if(SPWMPort3) {
			Port3Init(SPWMPort3);
			Port3SetOut(SPWMPort3);;
		}
		
		LPC_SYSCON->SYSAHBCLKCTRL |= 1<<8;			// Enable clock for Timer16B1
		LPC_TMR16B1->MCR = 0b11;					// Interrupt when timer matches MR0, and reset timer on match
		LPC_TMR16B1->MR0 = 0x57E;					// MR0 is 1406 i.e. 51200Hz = 1024 level resolution at 50Hz
		LPC_TMR16B1->PR = 0;						// Prescale is 0
		LPC_TMR16B1->TCR = 1;						// Enable Timer
		NVIC_EnableIRQ(TIMER_16_1_IRQn);
	}
	
	void SPWMStop(void) {
		LPC_TMR16B1->TCR = 0;						// Disable Timer
		LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<8);		// Disable clock for Timer16B1
		NVIC_DiableIRQ(TIMER_16_1_IRQn);			// Disable interrupt
		SPWMPort0 = 0;
		SPWMPort0 = 1;
		SPWMPort0 = 2;
		SPWMPort0 = 3;
	}
	
	void SPWMDuty(unsigned short channel, unsigned short value) {
		if(value > 1023) {
			value = 1023;
		}
		SPWMCycle[channel] = 1023 - value;
	}
	
	void TIMER16_1_IRQHandler(void) {
		LPC_TMR16B1->IR = LPC_TMR16B1->IR;			// Reset interrupt flag
		SPWMCount ++;
		if(SPWMCount >= 1024) {
			SPWMCount = 0;
			LPC_GPIO0->DATA &= ~SPWMPort0;
			LPC_GPIO1->DATA &= ~SPWMPort1;
			LPC_GPIO2->DATA &= ~SPWMPort2;
			LPC_GPIO3->DATA &= ~SPWMPort3;
		}

		#ifdef SPWM_0_0
			if(SPWMCount == SPWMCycle[SPWM_0_0]) LPC_GPIO0->DATA |= 1 << 0;
		#endif
		#ifdef SPWM_0_1
			if(SPWMCount == SPWMCycle[SPWM_0_1]) LPC_GPIO0->DATA |= 1 << 1;
		#endif
		#ifdef SPWM_0_2
			if(SPWMCount == SPWMCycle[SPWM_0_2]) LPC_GPIO0->DATA |= 1 << 2;
		#endif
		#ifdef SPWM_0_3
			if(SPWMCount == SPWMCycle[SPWM_0_3]) LPC_GPIO0->DATA |= 1 << 3;
		#endif
		#ifdef SPWM_0_4
			if(SPWMCount == SPWMCycle[SPWM_0_4]) LPC_GPIO0->DATA |= 1 << 4;
		#endif
		#ifdef SPWM_0_5
			if(SPWMCount == SPWMCycle[SPWM_0_5]) LPC_GPIO0->DATA |= 1 << 5;
		#endif
		#ifdef SPWM_0_6
			if(SPWMCount == SPWMCycle[SPWM_0_6]) LPC_GPIO0->DATA |= 1 << 6;
		#endif
		#ifdef SPWM_0_7
			if(SPWMCount == SPWMCycle[SPWM_0_7]) LPC_GPIO0->DATA |= 1 << 7;
		#endif
		#ifdef SPWM_0_8
			if(SPWMCount == SPWMCycle[SPWM_0_8]) LPC_GPIO0->DATA |= 1 << 8;
		#endif
		#ifdef SPWM_0_9
			if(SPWMCount == SPWMCycle[SPWM_0_9]) LPC_GPIO0->DATA |= 1 << 9;
		#endif
		#ifdef SPWM_0_10
			if(SPWMCount == SPWMCycle[SPWM_0_10]) LPC_GPIO0->DATA |= 1 << 10;
		#endif
		#ifdef SPWM_0_11
			if(SPWMCount == SPWMCycle[SPWM_0_11]) LPC_GPIO0->DATA |= 1 << 11;
		#endif
		#ifdef SPWM_1_0
			if(SPWMCount == SPWMCycle[SPWM_1_0]) LPC_GPIO1->DATA |= 1 << 0;
		#endif
		#ifdef SPWM_1_1
			if(SPWMCount == SPWMCycle[SPWM_1_1]) LPC_GPIO1->DATA |= 1 << 1;
		#endif
		#ifdef SPWM_1_2
			if(SPWMCount == SPWMCycle[SPWM_1_2]) LPC_GPIO1->DATA |= 1 << 2;
		#endif
		#ifdef SPWM_1_3
			if(SPWMCount == SPWMCycle[SPWM_1_3]) LPC_GPIO1->DATA |= 1 << 3;
		#endif
		#ifdef SPWM_1_4
			if(SPWMCount == SPWMCycle[SPWM_1_4]) LPC_GPIO1->DATA |= 1 << 4;
		#endif
		#ifdef SPWM_1_5
			if(SPWMCount == SPWMCycle[SPWM_1_5]) LPC_GPIO1->DATA |= 1 << 5;
		#endif
		#ifdef SPWM_1_6
			if(SPWMCount == SPWMCycle[SPWM_1_6]) LPC_GPIO1->DATA |= 1 << 6;
		#endif
		#ifdef SPWM_1_7
			if(SPWMCount == SPWMCycle[SPWM_1_7]) LPC_GPIO1->DATA |= 1 << 7;
		#endif
		#ifdef SPWM_1_8
			if(SPWMCount == SPWMCycle[SPWM_1_8]) LPC_GPIO1->DATA |= 1 << 8;
		#endif
		#ifdef SPWM_1_9
			if(SPWMCount == SPWMCycle[SPWM_1_9]) LPC_GPIO1->DATA |= 1 << 9;
		#endif
		#ifdef SPWM_1_10
			if(SPWMCount == SPWMCycle[SPWM_1_10]) LPC_GPIO1->DATA |= 1 << 10;
		#endif
		#ifdef SPWM_1_11
			if(SPWMCount == SPWMCycle[SPWM_1_11]) LPC_GPIO1->DATA |= 1 << 11;
		#endif
		#ifdef SPWM_2_0
			if(SPWMCount == SPWMCycle[SPWM_2_0]) LPC_GPIO2->DATA |= 1 << 0;
		#endif
		#ifdef SPWM_2_1
			if(SPWMCount == SPWMCycle[SPWM_2_1]) LPC_GPIO2->DATA |= 1 << 1;
		#endif
		#ifdef SPWM_2_2
			if(SPWMCount == SPWMCycle[SPWM_2_2]) LPC_GPIO2->DATA |= 1 << 2;
		#endif
		#ifdef SPWM_2_3
			if(SPWMCount == SPWMCycle[SPWM_2_3]) LPC_GPIO2->DATA |= 1 << 3;
		#endif
		#ifdef SPWM_2_4
			if(SPWMCount == SPWMCycle[SPWM_2_4]) LPC_GPIO2->DATA |= 1 << 4;
		#endif
		#ifdef SPWM_2_5
			if(SPWMCount == SPWMCycle[SPWM_2_5]) LPC_GPIO2->DATA |= 1 << 5;
		#endif
		#ifdef SPWM_2_6
			if(SPWMCount == SPWMCycle[SPWM_2_6]) LPC_GPIO2->DATA |= 1 << 6;
		#endif
		#ifdef SPWM_2_7
			if(SPWMCount == SPWMCycle[SPWM_2_7]) LPC_GPIO2->DATA |= 1 << 7;
		#endif
		#ifdef SPWM_2_8
			if(SPWMCount == SPWMCycle[SPWM_2_8]) LPC_GPIO2->DATA |= 1 << 8;
		#endif
		#ifdef SPWM_2_9
			if(SPWMCount == SPWMCycle[SPWM_2_9]) LPC_GPIO2->DATA |= 1 << 9;
		#endif
		#ifdef SPWM_2_10
			if(SPWMCount == SPWMCycle[SPWM_2_10]) LPC_GPIO2->DATA |= 1 << 10;
		#endif
		#ifdef SPWM_2_11
			if(SPWMCount == SPWMCycle[SPWM_2_11]) LPC_GPIO2->DATA |= 1 << 11;
		#endif
		#ifdef SPWM_3_0
			if(SPWMCount == SPWMCycle[SPWM_3_0]) LPC_GPIO3->DATA |= 1 << 0;
		#endif
		#ifdef SPWM_3_1
			if(SPWMCount == SPWMCycle[SPWM_3_1]) LPC_GPIO3->DATA |= 1 << 1;
		#endif
		#ifdef SPWM_3_2
			if(SPWMCount == SPWMCycle[SPWM_3_2]) LPC_GPIO3->DATA |= 1 << 2;
		#endif
		#ifdef SPWM_3_3
			if(SPWMCount == SPWMCycle[SPWM_3_3]) LPC_GPIO3->DATA |= 1 << 3;
		#endif
	}
	
#endif
 
/******************************************************************************
 * ADC Functions
 *****************************************************************************/
void ADCInit(unsigned char channels) {
	if(channels) {
		LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);			// Enable IOCONFIG
		
		if(channels & CHN0) LPC_IOCON->R_PIO0_11 = 0x02;            // Sets AD0 to Analogue mode
		if(channels & CHN1) LPC_IOCON->R_PIO1_0 = 0x02;             // Sets AD1 to Analogue mode
		if(channels & CHN2) LPC_IOCON->R_PIO1_1 = 0x02;             // Sets AD2 to Analogue mode
		if(channels & CHN3) LPC_IOCON->R_PIO1_2 = 0x02;             // Sets AD3 to Analogue mode
		if(channels & CHN4) LPC_IOCON->SWDIO_PIO1_3 = 0x02;         // Sets AD4 to Analogue mode
		if(channels & CHN5) LPC_IOCON->PIO1_4 = 0x01;               // Sets AD5 to Analogue mode
		if(channels & CHN6) LPC_IOCON->PIO1_10 = 0x01;              // Sets AD6 to Analogue mode
		if(channels & CHN7) LPC_IOCON->PIO1_11 = 0x01;              // Sets AD7 to Analogue mode
		
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);	// Enable clock to the ADC peripheral
		LPC_SYSCON->PDRUNCFG &= ~(1<<4);		// Enable power to the ADC peripheral
		
		#if ADC_MODE
			// set interrupt mode (ADC_MODE = 1)
			LPC_ADC->CR = 0x010F00 | channels;						// Sample at 4.5MHz clock, in burst mode, at 10bits
			LPC_ADC->INTEN = 0x100;									// Enable global interrupt
			NVIC_EnableIRQ(ADC_IRQn);
		#else
			LPC_ADC->CR = 0x000F00;									// Sample at 4.5MHz clock, in software start mode, at 10bits
			LPC_ADC->INTEN = 0x000;									// Disable global interrupt
		#endif
	}
}

void ADCStop(void) {
	NVIC_DisableIRQ(ADC_IRQn);
	Port0Init(PIN11);
	Port1Init(PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN10 | PIN11);
	LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<13);	// Enable clock to the ADC peripheral
	LPC_SYSCON->PDRUNCFG |= (1<<4);		// Enable power to the ADC peripheral
}

unsigned short ADCRead(unsigned char channel) {
	#if ADC_MODE
		if(channel & CHN0) return ((LPC_ADC->DR0 >> 6) & 0x3FF);
		else if(channel & CHN1) return ((LPC_ADC->DR1 >> 6) & 0x3FF);
		else if(channel & CHN2) return ((LPC_ADC->DR2 >> 6) & 0x3FF);
		else if(channel & CHN3) return ((LPC_ADC->DR3 >> 6) & 0x3FF);
		else if(channel & CHN4) return ((LPC_ADC->DR4 >> 6) & 0x3FF);
		else if(channel & CHN5) return ((LPC_ADC->DR5 >> 6) & 0x3FF);
		else if(channel & CHN6) return ((LPC_ADC->DR6 >> 6) & 0x3FF);
		else if(channel & CHN7) return ((LPC_ADC->DR7 >> 6) & 0x3FF);
		else return 0xffff;
	#else
		LPC_ADC->CR = 0x1000F00 | channel;			// Start now!
		while(!(LPC_ADC->GDR & (1 << 31)));			// Wait for done
		return ((LPC_ADC->GDR >> 6) & 0x3FF);
	#endif
}