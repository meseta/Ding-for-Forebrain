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
	int i, j;
	hid_device *handle;

    for(i=1; (i<argc && i<7); i++) {
		j = atoi(argv[i]);
		if(j > 255) {
            return 0;
		}
		buffer[i] = j;
	}

    handle = hid_open(0x1fc9, 0xbe11, NULL);	// attempt to open device Vendor ID 0x1fc9, Device ID 0x0003, no serial number
    if(handle) {
          hid_write(handle, buffer, 7);
    }

    return 0;
}
