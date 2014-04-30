/**
 * Arduino library for ADXL362
 * Datasheet can be found at http://www.analog.com/ADXL362
 *
 * Copyright 2014  zachwick <zach@zachwick.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */ 

#include <Arduino.h>
#include <ADXL362.h>
#include <SPI.h>

ADXL362::ADXL362(void) {
	_pin = 4;
	_debug = false;
	pinMode (_pin, OUTPUT);
}

ADXL362::ADXL362(int cs_pin) {
	_pin = cs_pin;
	_debug = false;
	pinMode (_pin, OUTPUT);
}

ADXL362::ADXL362(int cs_pin, boolean debugSerial) {
	_pin = cs_pin;
	_debug = debugSerial;
	pinMode (_pin, OUTPUT);
}

void
ADXL362::begin(void) {
	SPI.begin();
	// SPI_MODE0 means that CPHA = 0 and CPOL = 0
	SPI.setDataMode (SPI_MODE0);
	// Set the SPI bus to operate at 4MHz since the ADXL362 can do [1MHz - 5MHz]
	SPI.setClockDivider (SPI_CLOCK_DIV4);

	delay (1000);
    
	// ADXL362's reset is writing ASCII "R" to 0x1F
	write_single_register (SOFT_RESET, 0x52);

	if (_debug) {
		Serial.print ("Soft Reset\n");
	}

	delay (10);
}

void
ADXL362::beginMeasure(void) {
	byte temporary = read_single_register (POWER_CTL);
	byte tempwrite = temporary | MEASURE_MODE_ON;
	write_single_register (POWER_CTL, tempwrite);
	
	if (_debug) {
		Serial.print ("MEASURE_MODE_ON ");
		Serial.print ("POWER_CTL written as: ");
		Serial.print (String (tempwrite, HEX));
		Serial.print ("\n");
	}
	
	delay (10);	
}

int
ADXL362::readXData(int include_sx_bits) {
	int xdata = read_multiple_registers (XDATA_L);

	if (!include_sx_bits) {
		int sx_mask = 0xF000;
		xdata &= (~sx_mask);
	}

	if (_debug) {
		Serial.print ("X-axis: ");
		Serial.print (String(xdata, HEX));
		Serial.print ("\n");
	}

	return xdata;
}

byte
ADXL362::readXMSB(void) {
	int xdata = read_single_register (XDATA);

	if (_debug) {
		Serial.print ("X-axis MSB: ");
		Serial.print (String(xdata, HEX));
		Serial.print ("\n");
	}	

	return xdata;
}

int
ADXL362::readYData(int include_sx_bits) {
	int ydata = read_multiple_registers (YDATA_L);

	if (!include_sx_bits) {
		int sx_mask = 0xF000;
		ydata &= (~sx_mask);
	}

	if (_debug) {
		Serial.print ("Y-axis: ");
		Serial.print (String(ydata, HEX));
		Serial.print ("\n");
	}

	return ydata;
}

byte
ADXL362::readYMSB(void) {
	int ydata = read_single_register (YDATA);

	if (_debug) {
		Serial.print ("Y-axis MSB: ");
		Serial.print (String(ydata, HEX));
		Serial.print ("\n");
	}

	return ydata;
}

int
ADXL362::readZData(int include_sx_bits) {
	int zdata = read_multiple_registers (ZDATA_L);

	if (!include_sx_bits) {
		int sx_mask = 0xF000;
		zdata &= (~sx_mask);
	}

	if (_debug) {
		Serial.print ("Z-axis: ");
		Serial.print (String(zdata, HEX));
		Serial.print ("\n");
	}
	
	return zdata;
}

byte
ADXL362::readZMSB(void) {
	int zdata = read_single_register (ZDATA);

	if (_debug) {
		Serial.print ("Z-axis MSB: ");
		Serial.print (String(zdata, HEX));
		Serial.print ("\n");
	}

	return zdata;
}

int
ADXL362::readTemp(int include_sx_bits) {
	int temperature = read_multiple_registers (TEMP_L);

	if (!include_sx_bits) {
		int sx_mask = 0xF000;
		temperature &= (~sx_mask);
	}

	if (_debug) {
		Serial.print ("Temperature: ");
		Serial.print (String(temperature, HEX));
		Serial.print ("\n");
	}

	return temperature;
}

void
ADXL362::readXYZTData(int &XData, int &YData, int &ZData, int &Temperature, int include_sx_bits){
	byte x_l, x_h, y_l, y_h, z_l, z_h;
	byte temp_l, temp_h;
	// The actual X,Y,Z axis data is only 12 bits. The remaining 4 bits are "sign
	// extension" bits and are always equal to the most significant bit.
	// They are masked to zero so as to return the correct value back by reference.
	byte sx_mask = 0xF0;

	// burst SPI read
	// A burst read of all three axis is required to guarantee all measurements correspond to same sample time
	digitalWrite (_pin, LOW);
	SPI.transfer (READ);
	SPI.transfer (XDATA_L);  // Start at XDATA_L register

	x_l = SPI.transfer (0x00);
	x_h = SPI.transfer (0x00);
	if (!include_sx_bits) {
		x_h &= (~sx_mask);
	}
	XData = x_l + (x_h << 8);

	y_l = SPI.transfer (0x00);
	y_h = SPI.transfer (0x00);
	if (!include_sx_bits) {
		y_h &= (~sx_mask);
	}
	YData = y_l + (y_h <<  8);

	z_l = SPI.transfer (0x00);
	z_h = SPI.transfer (0x00);
	if (!include_sx_bits) {
		z_h &= (~sx_mask);
	}
	ZData = z_l + (z_h << 8);

	temp_l = SPI.transfer (0x00);
	temp_h = SPI.transfer (0x00);
	if (!include_sx_bits) {
		temp_h &= (~sx_mask);
	}
	Temperature = temp_l + (temp_h << 8);

	digitalWrite (_pin, HIGH);
}

void
ADXL362::set_filter_ctl (byte settings_mask) {
	write_single_register(byte(FILTER_CTL), byte(settings_mask));

	if (_debug) {
		Serial.print ("FILTER_CTL written as: ");
		Serial.print (String (byte(settings_mask),HEX));
		Serial.print ("\n");
	}
}

void
ADXL362::set_power_ctl (byte settings_mask) {
	write_single_register ((byte)POWER_CTL, (byte)settings_mask);

	if (_debug) {
		Serial.print ("POWER_CTL written as: ");
		Serial.print (String (byte(settings_mask),HEX));
		Serial.print ("\n");
	}
}

void
ADXL362::do_self_test (void) {
	write_single_register ((byte)SELF_TEST, (byte)SELF_TEST_ON);

	if (_debug) {
		Serial.print ("Triggering SELF_TEST\n");
	}
}

byte
ADXL362::read_single_register(byte address) {
	byte value = 0;
  
	digitalWrite (_pin, LOW);
	SPI.transfer (READ);  // read instruction
	SPI.transfer (address);
	value = SPI.transfer (0x00);
	digitalWrite (_pin, HIGH);

	if (_debug) {
		Serial.print ("READING Address: ");
		Serial.print (String (address,HEX));
		Serial.print (" Value: ");
		Serial.print (String (value,HEX));
		Serial.print ("\n");
	}

	return value;
}

void
ADXL362::write_single_register(byte address, byte value) {
  
	digitalWrite (_pin, LOW);
	SPI.transfer (WRITE);  // write instruction
	SPI.transfer (address);
	SPI.transfer (value);
	digitalWrite (_pin, HIGH);

	if (_debug) {
		Serial.print ("WRITING Address: ");
		Serial.print (String (address,HEX));
		Serial.print (" Value: ");
		Serial.print (String (value,HEX));
		Serial.print ("\n");
	}
}

int
ADXL362::read_multiple_registers(byte address) {
	byte value_l = 0;
	byte value_h = 0;
  
	digitalWrite (_pin, LOW);
	SPI.transfer (READ);  // read instruction
	SPI.transfer (address);  
	value_l = SPI.transfer (0x00);
	value_h = SPI.transfer (0x00);
	digitalWrite (_pin, HIGH);

	if (_debug) {
		Serial.print ("READING Address: ");
		Serial.print (String (address,HEX));
		Serial.print (" Value LOW BYTE: ");
		Serial.print (String (value_l,HEX));
		Serial.print (" Value HIGH BYTE: ");
		Serial.print (String (value_h,HEX));
		Serial.print ("\n");
	}

	return value_l + (value_h << 8);
}

