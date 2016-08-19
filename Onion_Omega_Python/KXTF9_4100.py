# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# KXTF9-4100
# This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# KXTF9-4100 address, 0x0F(15)
# Select Control register1, 0x1B(27)
#		0xD0(208)	Operating mode, 12-bit valid
i2c.writeByte(0x0F, 0x1B, 0xD0)

time.sleep(0.5)

# KXTF9-4100 address, 0x0F(15)
# Read data back from 0x06(06), 6 bytes
# xAccl LSB, xAccl MSB, yAccl LSB, yAccl MSB, zAccl LSB, zAccl MSB
data = i2c.readBytes(0x0F, 0x06, 6)

# Convert the data to 12-bits
xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16
if xAccl > 2047 :
	xAccl -= 4096
yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16
if yAccl > 2047 :
	yAccl -= 4096
zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16
if zAccl > 2047 :
	zAccl -= 4096

# Output data to screen
print "Acceleration in X-Axis : %d" %xAccl
print "Acceleration in Y-Axis : %d" %yAccl
print "Acceleration in Z-Axis : %d" %zAccl
