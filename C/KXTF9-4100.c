// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KXTF9-4100
// This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main()
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0)
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, KXTF9-4100 I2C address is 0x0F(15)
	ioctl(file, I2C_SLAVE, 0x0F);

	// Select Control register1(0x1B)
	// Operating mode, 12-bit valid
	char config[2] = {0};
	config[0] = 0x1B;
	config[1] = 0xD0;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from 0x06(06)
	// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
	char reg[1] = {0x06};
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else 
	{
		// Convert the data to 12-bits
		int xAccl = (data[1] * 256 + (data[0] & 0xF0)) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}

		int yAccl = (data[3] * 256 + (data[2] & 0xF0)) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}

		int zAccl = (data[5] * 256 + (data[4] & 0xF0)) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Output data to screen
		printf("Acceleration in X-Axis : %d \n", xAccl);
		printf("Acceleration in Y-Axis : %d \n", yAccl);
		printf("Acceleration in Z-Axis : %d \n", zAccl);
	}
}
