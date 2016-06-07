// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KXTF9-4100
// This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class KXTF9_4100
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, KXTF9-4100 I2C address is 0x0F(15)
		I2CDevice device = bus.getDevice(0x0F);

		// Select Control register1, 0x1B(27)
		// Operating mode, 12-bit valid
		device.write(0x1B, (byte)0xD0);
		Thread.sleep(500);

		// Read 6 bytes of data from 0x06(06)
		// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
		byte[] data = new byte[6];
		device.read(0x06, data, 0, 6);

		// Convert the data to 12-bits
		int xAccl = ((data[1] & 0xFF) * 256 + (data[0] & 0xF0)) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}

		int yAccl = ((data[3] & 0xFF) * 256 + (data[2] & 0xF0)) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}

		int zAccl = ((data[5] & 0xFF) * 256 + (data[4] & 0xF0)) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Output data to screen
		System.out.printf("Acceleration in X-Axis : %d %n", xAccl);
		System.out.printf("Acceleration in Y-Axis : %d %n", yAccl);
		System.out.printf("Acceleration in Z-Axis : %d %n", zAccl);
	}
}