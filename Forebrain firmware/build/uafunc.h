/******************************************************************************
 * Version 1
 
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/
 
#ifndef __UAFUNC_H__
#define __UAFUNC_H__
#include "config.h"
#define WEAK_ALIAS(f) __attribute__ ((weak, alias (#f)));
#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((alias (#f)));

#define TRUE		1
#define YES			1
#define ON			1
#define ENABLE		1
#define FALSE		0
#define NO			0
#define OFF			0
#define DISABLE		0

extern void GeneralFault(void);

/******************************************************************************
 * Misc Functions
 * Miscellaneous useful functions
 *
 * delay() - delay in milliseconds.  This function is very approximate, do not
 * 		use for accurate timing
 *****************************************************************************/
void Delay(unsigned int milliseconds);

#if IAP_EN

	typedef void (*IAP)(unsigned int [], unsigned int []);
	unsigned int IAPCommand[5], IAPResult[4];
	void Reprogram(void);
#endif

/******************************************************************************
 * Clock Functions
 * These functions set up the LPC1343's clocks, systick, and watchdog timer
 *
 * SysTickInit() and SysTickStop() - sets up or stops the SysTick timer, which
 *		will generate an interrupt every 10ms, the function SysTick_Handler()
 *		handles the interrupt (the user should supply this function).  The
 *		interrupt period can be changed in config.h
 * WDTInit() - sets up the watchdog timer, which will either generate an
 *		and interrupt, or reset the chip if it times out without being "fed"
 *		in the case of interrupt, the user should supply the WDT_IRQHandler()
 *		function.  The watchdog timer is supplied with the timeout time in ms
 *		from 1ms up to a maximum of 4 hours
 * WDTFeed() - Feeds the watchdog to prevent interrupt or reset
 *****************************************************************************/
// Clocks
#if SYSTICK_EN
	void SysTickInit(void);
	void SysTickStop(void);
	volatile unsigned int SysTicks, SysTickDelayTicks;
	
	static inline void SysTickEmpty(void) { return; }
	void Tick(void) WEAK_ALIAS(SysTickEmpty);
#endif

#if WDT_EN
	void WDTInit(void);
	static inline void WDTFeed(void) {	LPC_WDT->FEED = 0xAA; LPC_WDT->FEED = 0x55;	}
#endif
/******************************************************************************
 * Power mode Functions
 * These functions use the LPC1343's power modes, reset, and BOD modes
 *****************************************************************************/

/******************************************************************************
 * GPIO Functions
 * These functions use the digital input and output functions, the functions
 * for each port are separated.
 * 
 * Port#Init() - set up the specified  pins for that port as input/outputs
 * 		with pull-up resistors enabled by default (where approriate)
 *		note: running Port0Init(ALL) or Port#Init(PIN0) will set Port 0 Pin 0
 * 			to digital IO mode, this means that the RESET button will no longer
 *			cause a reset (this can be desireable so that the RESET button can
 *			be used as a regular button).  To avoid this, run
 *		GPIO0Init(ALL & ~PIN0);
 * 			note: Port#Init() for all four ports are run for ALL (except
 *			Port 0 Pin 0) automatically at start/reset, so it is not necessary
 *			to run at the start of user code
 * Port#SetIn() and Port#SetOut() - set input or output of specified 
 * Port#Write() - set specified pin states
 * Port#Toggle() - toggle specified pin state
 * Port#Read() - read specified pin states
 * Port#Hysteresis() - turn on or off hysteresis for specified pins
 * Port#Resistor() - pull-up/down resistors for specified pins
 * 		note: Port 0 pins 4 and 5 do not have hysteresis and pull-up/down
 *		resistors
 *
 * LEDon() and LEDoff() - turn on and off the specified LEDs on Port 3
 * LEDtoggle() - toggle the specified LEDs on Port 3

 *****************************************************************************/
#define PIN0		0x001
#define PIN1		0x002
#define PIN2		0x004
#define PIN3		0x008
#define PIN4		0x010
#define PIN5		0x020
#define PIN6		0x040
#define PIN7		0x080
#define PIN8		0x100
#define PIN9		0x200
#define PIN10		0x400
#define PIN11		0x800
#define LED0		0x001
#define LED1		0x002
#define LED2		0x004
#define LED3		0x008
#define ALL			0xfff

#define PULL_UP		0x10
#define PULL_DOWN	0x01
#define REPEATER	0x11

void Port0Init(unsigned short pins);
void Port1Init(unsigned short pins);
void Port2Init(unsigned short pins);
void Port3Init(unsigned short pins);

static inline void Port0SetIn(unsigned short pins) {	LPC_GPIO0->DIR &= ~pins;	}
static inline void Port1SetIn(unsigned short pins) {	LPC_GPIO1->DIR &= ~pins;	}
static inline void Port2SetIn(unsigned short pins) {	LPC_GPIO2->DIR &= ~pins;	}
static inline void Port3SetIn(unsigned short pins) {	LPC_GPIO3->DIR &= ~pins;	}
static inline void Port0SetOut(unsigned short pins) {	LPC_GPIO0->DIR |= pins;	}
static inline void Port1SetOut(unsigned short pins) {	LPC_GPIO1->DIR |= pins;	}
static inline void Port2SetOut(unsigned short pins) {	LPC_GPIO2->DIR |= pins;	}
static inline void Port3SetOut(unsigned short pins) {	LPC_GPIO3->DIR |= pins;	}

static inline void Port0Write(unsigned short pins, unsigned char value) {
	if(value) LPC_GPIO0->DATA |= pins;
	else LPC_GPIO0->DATA &= ~pins;
}
static inline void Port1Write(unsigned short pins, unsigned char value) {
	if(value) LPC_GPIO1->DATA |= pins;
	else LPC_GPIO1->DATA &= ~pins;
}
static inline void Port2Write(unsigned short pins, unsigned char value) {
	if(value) LPC_GPIO2->DATA |= pins;
	else LPC_GPIO2->DATA &= ~pins;
}
static inline void Port3Write(unsigned short pins, unsigned char value) {
	if(value) LPC_GPIO3->DATA |= pins;
	else LPC_GPIO3->DATA &= ~pins;
}

static inline void Port0Toggle(unsigned short pins) {	LPC_GPIO0->DATA ^= pins;	}
static inline void Port1Toggle(unsigned short pins) {	LPC_GPIO1->DATA ^= pins;	}
static inline void Port2Toggle(unsigned short pins) {	LPC_GPIO2->DATA ^= pins;	}
static inline void Port3Toggle(unsigned short pins) {	LPC_GPIO3->DATA ^= pins;	}

static inline unsigned char Port0Read(unsigned short pins) {
	if(LPC_GPIO0->DATA & pins) return 1;
	else return 0;
}
static inline unsigned char Port1Read(unsigned short pins) {
	if(LPC_GPIO1->DATA & pins) return 1;
	else return 0;
}
static inline unsigned char Port2Read(unsigned short pins) {
	if(LPC_GPIO2->DATA & pins) return 1;
	else return 0;
}
static inline unsigned char Port3Read(unsigned short pins) {
	if(LPC_GPIO3->DATA & pins) return 1;
	else return 0;
}

static inline void LEDOn(unsigned short pins) {	LPC_GPIO3->DATA &= ~pins;	}
static inline void LEDOff(unsigned short pins) {	LPC_GPIO3->DATA |= pins;	}
static inline void LEDToggle(unsigned short pins) {	LPC_GPIO3->DATA ^= pins;	}
static inline void LEDInit(unsigned short pins) {	Port3Init(pins);	Port3SetOut(pins);	LEDOff(pins);	}

void Port0Hysteresis(unsigned short pins, unsigned char value);
void Port1Hysteresis(unsigned short pins, unsigned char value);
void Port2Hysteresis(unsigned short pins, unsigned char value);
void Port3Hysteresis(unsigned short pins, unsigned char value);

void Port0Pull(unsigned short pins, unsigned char value);
void Port1Pull(unsigned short pins, unsigned char value);
void Port2Pull(unsigned short pins, unsigned char value);
void Port3Pull(unsigned short pins, unsigned char value);

// interrupts

/******************************************************************************
 * USB HID functions
 * These functions make use of LPC1343's HID ROM code  to communicate with a
 * computer over USB HID, simply run USBInit() to initialise (and USBStop() to
 * disable.  Data can then be sent to the computer using USBIn(), and received
 * from the computer using USBOut().  The user should write their own USBIn()
 * and USBOut() function.
 * 
 * There is no need to call these functions from within user code: USBIn() will
 * be automatically triggered (by USB polling from the computer); USBOut() will
 * automatically run when Forebrain receives data from the computer
 *
 * The Vendor ID, Product ID, Device ID, can be changed in config.h,
 * manufacturer descriptor string, product descriptor string, and serial number
 * string can also be changed in the page uafunc.c
 *
 * WARNING: do NOT use Timer 32B1 when using USB HID
 *****************************************************************************/
#if USB_EN
	void USBIn(unsigned char USBdata[], unsigned int USBlength);
	void USBOut(unsigned char USBdata[], unsigned int USBlength);
	void USBInit(void);
	void USBStop(void);

	typedef	struct {
		unsigned short USBidVendor;
		unsigned short USBidProduct;
		unsigned short USBbcdDevice;
		unsigned int USBstrDescPtr;
		unsigned char USBinReportCount;
		unsigned char USBoutReportCount;
		unsigned char USBsampleInterval;
		unsigned int USBInPtr;
		unsigned int USBOutPtr;
	} HID_DEV_INFO;

	typedef struct {
		unsigned short devType;
		unsigned int devDetailPtr;
	} USB_DEV_INFO; 

	typedef	struct {
		void (*init_clk_pins)(void);
		void (*isr)(void);
		void (*init)(const USB_DEV_INFO * DevInfoPtr); 
		void (*connect)(unsigned int con);
	} USBD;

	typedef	struct {
		const USBD * pUSBD;
	}  ROM;

	void USBConnect(void);
	void USBDisconnect(void);
#endif
/******************************************************************************
 * UART Functions
 * These functions allow the LPC1343 to communicate over its UART hardware
 *****************************************************************************/

/******************************************************************************
 * I2C Functions
 * These functions allow the LPC1343 to communicate over its I2C hardware in
 * master mode.  Use I2CInit() and I2CStop() to initialise or stop
 * 
 * To use I2C to send or receive data, set the variables I2CWriteLength,
 * I2CReadLength, I2CMasterBuffer and then run I2CEngine();
 *
 * EEPROMRead() - read a byte of data from the address,
 * EEPROMWrite() - write a byte of data to the address
 * 		note: the 24FC256 takes a maximum of 5ms to commit the write, avoid
 * 			performing any EEPROM reads/writes within 5ms of the previous write
 *****************************************************************************/
#if I2C_EN
	#define I2C_IDLE			0
	#define I2C_STARTED			1
	#define I2C_RESTARTED		2
	#define I2C_REPEATED_START	3
	#define I2C_ACK				4
	#define I2C_NACK			5
	#define I2C_NACK_END		6
	#define I2C_ERROR			8

	#define I2C_AA				0x04
	#define I2C_SI				0x08
	#define I2C_STO				0x10
	#define I2C_STA				0x20
	#define I2C_ENA				0x40
	
	#define MASTER				0
	#define SLAVE				1

	volatile unsigned char I2CMasterState, I2CMasterState2, I2CSlaveState;
	volatile unsigned char I2CWriteData[I2C_DATA_SIZE], I2CReadData[I2C_DATA_SIZE];
	volatile unsigned int I2CReadLength, I2CWriteLength;
	volatile unsigned int I2CReadIndex, I2CWriteIndex;

	void I2CInit(unsigned short speed, unsigned char mode);
	void I2CStop(void);
	void I2CEngine(void);

	void EEPROMWrite(unsigned short address, unsigned char data);
	unsigned char EEPROMRead(unsigned short address);
	
	void EEPROMBulkWrite(unsigned short address, unsigned char *data, unsigned int length);
	void EEPROMBulkRead(unsigned short address, unsigned char *data, unsigned int length);
#endif

/******************************************************************************
 * SSP/SPI Functions
 * These functions allows the LPC1343 to communicate over its SSP/SPI hardware
 *****************************************************************************/

/******************************************************************************
 * Timer Functions
 * These functions set up the LPC1343's timer hardware (including for PWM)
 *****************************************************************************/

// Mode: 0=timer, 1=counter, 2=hardware PWM
// void T16B0Init(unsigned char mode, unsigned short value);
// void T32B0Init(unsigned char mode, unsigned short value);
// void T16B1Init(unsigned char mode, unsigned short value);

#if SPWM_EN
	// SPWM50: 10-bit PWM at 50Hz
	volatile unsigned short SPWMCycle[40], SPWMCount, SPWMPort0, SPWMPort1, SPWMPort2, SPWMPort3;
	void SPWMInit(void);
	void SPWMDuty(unsigned short channel, unsigned short value);
#else
	//void T16B1Init(unsigned char mode, unsigned short value);
#endif


/******************************************************************************
 * ADC Functions
 * These functions use the LPC1343's analogue to digital converter
 *****************************************************************************/
#define CHN0		0x01
#define CHN1		0x02
#define CHN2		0x04
#define CHN3		0x08
#define CHN4		0x10
#define CHN5		0x20
#define CHN6		0x40
#define CHN7		0x80

#define ADC_VAL		((LPC_ADC->GDR >> 6) & 0x3FF)
#define ADC_CHAN	((LPC_ADC->GDR >> 24) & 0x7)

void ADCInit(unsigned char channels); 
void ADCStop(void);
unsigned short ADCRead(unsigned char channel);
 
#endif /* __UAFUNC_H__ */