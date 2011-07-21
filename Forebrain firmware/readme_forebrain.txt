Current version: 0.1 (2011/7/21)

This is the template code for use with Forebrain development boards, included 
are the following files:

.cproject				Eclipse project file
.project				Eclipse project file
config.h				Configuration constants
license.txt				License information
main.c					Your code here!
readme_forebrain.txt	This file
build/core_cm3.c		Core LPC1343 file
build/core_cm3.h		Core LPC1343 file
build/linker.ld			Linker script
build/LPC13xx.h			Core LPC1343 file
build/Makefile			Compiler script
build/startup.c			Core LPC1343 file
build/uafunc.c			Universal Air Forebrain functions
build/uafunc.h			Universal Air Forebrain functions

TODO:
	-I2C Slave mode
	-UART
	-SSP
	-Timers
	-Hardware PWM
	-CRP
	-Power modes
	

Known issues:
	-EEPROMBulkWrite can only write up to 7 bytes at a time

Changelog:

Version 0.1 (2011/7/21):
	Original version