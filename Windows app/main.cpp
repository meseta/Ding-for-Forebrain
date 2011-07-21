/******************************************************************************
 * Version 1
 
 * Copyright (c) 2011, Universal Air Ltd. All rights reserved.
 * Source and binaries are released under the BSD 3-Clause license
 * See license.txt files for the text of the license
 ******************************************************************************/

//#include <windows.h>
#include "hidapi.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
	unsigned char buffer[7] = {0,0,0,0,0,0,0};
	unsigned int i;
	hid_device *handle;

	if(argc > 1) {
		i = atoi(argv[1]);
		if(i > 255) {
            return 0;
		}
		buffer[1] = i;
	}

    handle = hid_open(0x1fc9, 0xbe11, NULL);	// attempt to open device Vendor ID 0x1fc9, Device ID 0x0003, no serial number
    if(handle) {
          hid_write(handle, buffer, 7);
    }

    return 0;
}
