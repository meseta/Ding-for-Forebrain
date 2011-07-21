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
 *****************************************************************************/
 
#if IAP_EN
	typedef void (*IAP)(unsigned int [], unsigned int []);
	unsigned int IAPCommand[5], IAPResult[4];
	void Reprogram(void);
#endif

/******************************************************************************
 * Clock Functions
 *****************************************************************************/

void Delay(unsigned int milliseconds);
void WaitDelay(unsigned int milliseconds);
 
#if SYSTICK_EN
	void SysTickInit(void);
	void SysTickStop(void);
	volatile unsigned int SysTicks, SysTickDelayTicks;
	
	static inline void SysTickEmpty(void) { return; }
	void Tick(void) WEAK_ALIAS(SysTickEmpty);
	
	void SysTickDelay(unsigned int milliseconds);
#endif

#if WDT_EN
	void WDTInit(void);
	static inline void WDTFeed(void) {	LPC_WDT->FEED = 0xAA; LPC_WDT->FEED = 0x55;	}
#endif

/******************************************************************************
 * Power mode Functions
 *****************************************************************************/

/******************************************************************************
 * GPIO Functions
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
 *****************************************************************************/
#if USB_EN
	void USBIn(unsigned char USBData[], unsigned int USBLength);
	void USBOut(unsigned char USBData[], unsigned int USBLength);
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
 *****************************************************************************/

/******************************************************************************
 * I2C Functions
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
 *****************************************************************************/

/******************************************************************************
 * Timer Functions
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