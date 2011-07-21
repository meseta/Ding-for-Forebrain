/******************************************************************************
 * Version 1
 
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/
 
#ifndef __CONFIG_H__
#define __CONFIG_H__
 
 
/******************************************************************************
 * Misc Config
 *****************************************************************************/
#define IAP_EN				1			// Set to 1 to enable the IAP functions like Reprogram() (set to 0 to disable and save some RAM)
 
 
 /******************************************************************************
 * Clock Config
 *****************************************************************************/
#define SYSTICK_EN			1			// Set to 1 to enable SysTick (set to 0 to disable and save some RAM)
#define SYSTICK_MS			1			// SysTick period in milliseconds

#define WDT_EN				0			// Set to 1 to enable WDT (set to 0 to disable and save some RAM)
#define WDT_MS				1000		// WDT period in milliseconds (max 14400000 = 4 hours
#define WDT_MODE			0			// WDT mode: 0=reset, 1=interrupt

/******************************************************************************
 * Power mode Config
 *****************************************************************************/


/******************************************************************************
 * USB HID Config
 ******************************************************************************/
#define USB_EN				1			// Set to 1 to enable USB HID

#define USB_VENDOR_ID		0x1FC9		// The default NXP Vendor ID is 0x1FC9
#define USB_PRODUCT_ID		0xbe11
#define USB_DEVICE_ID		0x0001
#define USB_IN_BYTES		2			// The number of bytes for the input report (Forebrain to computer) (max. 64)
#define USB_OUT_BYTES		6			// The number of bytes for the output report (computer to Forebrain) (max. 64)
#define USB_SAMPLE			255			// Sample interval (in ms - up to 255, although it looks like 33ms is max)

/******************************************************************************
 * UART Config
 *****************************************************************************/

/******************************************************************************
 * I2C Config
 *****************************************************************************/
#define I2C_EN				1			// Set to 1 to enable I2C (set to 0 to disable and save some RAM)
 
#define I2C_DATA_SIZE		50			// maximum size of data buffers
#define I2C_TIMEOUT			0xFFFFF		// Timeout for I2C

#define I2C_SLAVE_ADR0		0x00		// Slave address 0 when Forebrain is in slave mode
#define I2C_SLAVE_ADR1		0x00		// Slave address 1 when Forebrain is in slave mode
#define I2C_SLAVE_ADR2		0x00		// Slave address 2 when Forebrain is in slave mode
#define I2C_SLAVE_ADR3		0x00		// Slave address 3 when Forebrain is in slave mode

#define I2C_EEPROM_ADR		0xA0		// address of the 24FC256 EEPROM on Forebrain

/******************************************************************************
 * SSP/SPI Config
 *****************************************************************************/

/******************************************************************************
 * Timer Config
 *****************************************************************************/
#define SPWM_EN_EN		0				// Set to 1 to enable Software PWM (set to 0 to disable and save some RAM)
// Uncomment any of the following to enable Software PWM on that channel
#define SPWM_0_0		0				// Software PWM on Port 0, pin 0
#define SPWM_0_1		1				// Software PWM on Port 0, pin 1
#define SPWM_0_2		2				// Software PWM on Port 0, pin 2
#define SPWM_0_3		3				// Software PWM on Port 0, pin 3
#define SPWM_0_4		4				// Software PWM on Port 0, pin 4
#define SPWM_0_5		5				// Software PWM on Port 0, pin 5
#define SPWM_0_6		6				// Software PWM on Port 0, pin 6
#define SPWM_0_7		7				// Software PWM on Port 0, pin 7
#define SPWM_0_8		8				// Software PWM on Port 0, pin 8
#define SPWM_0_9		9				// Software PWM on Port 0, pin 9
#define SPWM_0_10		10				// Software PWM on Port 0, pin 10
#define SPWM_0_11		11				// Software PWM on Port 0, pin 11
#define SPWM_1_0		12				// Software PWM on port 1, pin 0
#define SPWM_1_1		13				// Software PWM on port 1, pin 1
#define SPWM_1_2		14				// Software PWM on port 1, pin 2
#define SPWM_1_3		15				// Software PWM on port 1, pin 3
#define SPWM_1_4		16				// Software PWM on port 1, pin 4
#define SPWM_1_5		17				// Software PWM on port 1, pin 5
#define SPWM_1_6		18				// Software PWM on port 1, pin 6
#define SPWM_1_7		19				// Software PWM on port 1, pin 7
#define SPWM_1_8		20				// Software PWM on port 1, pin 8
#define SPWM_1_9		21				// Software PWM on port 1, pin 9
#define SPWM_1_10		22				// Software PWM on port 1, pin 10
#define SPWM_1_11		23				// Software PWM on port 1, pin 11
#define SPWM_2_0		24				// Software PWM on port 2, pin 0
#define SPWM_2_1		25				// Software PWM on port 2, pin 1
#define SPWM_2_2		26				// Software PWM on port 2, pin 2
#define SPWM_2_3		27				// Software PWM on port 2, pin 3
#define SPWM_2_4		28				// Software PWM on port 2, pin 4
#define SPWM_2_5		29				// Software PWM on port 2, pin 5
#define SPWM_2_6		30				// Software PWM on port 2, pin 6
#define SPWM_2_7		31				// Software PWM on port 2, pin 7
#define SPWM_2_8		32				// Software PWM on port 2, pin 8
#define SPWM_2_9		33				// Software PWM on port 2, pin 9
#define SPWM_2_10		34				// Software PWM on port 2, pin 10
#define SPWM_2_11		35				// Software PWM on port 2, pin 11
#define SPWM_3_0		36				// Software PWM on port 3, pin 0
#define SPWM_3_1		37				// Software PWM on port 3, pin 1
#define SPWM_3_2		38				// Software PWM on port 3, pin 2
#define SPWM_3_3		39				// Software PWM on port 3, pin 3

/******************************************************************************
 * ADC Config
 *****************************************************************************/
#define ADC_MODE			0			// ADC mode (0=normal, 1=interrupt)










#endif // __CONFIG_H__