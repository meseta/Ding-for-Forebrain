/******************************************************************************
 * Version 1
 
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/

#include "lpc13xx.h"
#include "uafunc.h"

#define MAX_DINGTIME	100			// we want to set a maximum "dingtime" to avoid drawing too much current from USB
volatile unsigned char dingtime;	// this variable stores the duration of the pulse that drives the solenoid
volatile unsigned char reprogram;


int main(void) {
	Port2SetOut(PIN11);				// Port 2 Pin 11 connects to the transistor that drives the solenoid
	Port2Write(PIN11, 0);			// set this pin to zero initially, setting it to 1 enables the solenoid
	
	Delay(100);
	
	I2CInit(400, MASTER);			// Initialise the I2C peripheral as master at 400kHz, this is needed for communications with the on-board EEPROM
	dingtime = EEPROMRead(0);		// We are storing the "dingtime" at address zero on the EEPROM
	
	if(dingtime > MAX_DINGTIME) {	// if we find the stored value is too high (i.e. most likely invalid data), we default to half the max value
		dingtime = MAX_DINGTIME/2;
	}
	
	dingtime = 60;
	
	USBInit();
	reprogram = 0;
	
	while (1) {
	
		// ****** Reprogram
		if(reprogram == 1) {
			USBDisconnect();
			Delay(500);
			Reprogram();
		}
	}
}

void USBIn(unsigned char USBData[], unsigned int USBLength) {
	USBData[0] = 0;
	USBData[1] = 0;
}

void USBOut(unsigned char USBData[], unsigned int USBLength) {
	unsigned char command = USBData[0];
	// Forebrain receives data from host computer
	switch(command) {
		case 0:
			// DING plx!
			Port2Write(PIN11, 1);
			WaitDelay(dingtime);	// we are using WaitDelay since it is slightly safer than Delay() when SysTick is enabled
			Port2Write(PIN11, 0);
			break;
		case 254:
			// update dingtime
			// USBdata[1]: the dingtime (0 to MAX_DINGTIME)
			dingtime = USBData[1];
			if(dingtime > MAX_DINGTIME) {
				dingtime = MAX_DINGTIME;
			}
			EEPROMWrite(0, dingtime);
			break;
		case 255:
			// reinvoke ISP
			// because forebrain is most likely stuck to the bottom of a bell 
			// by now, we give the user the ability to reinvoke programming
			// mode to allow user an easy way to update firmware
			// note: not a good idea attempting to enter reprogramming mode in
			// the middle of the USB routine!
			reprogram = 1;
			break;
	}
}
