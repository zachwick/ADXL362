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

#include "Arduino.h"

#ifndef ADXL362_h
#define ADXL362_h

// SPI Commands
#define WRITE 0x0A
#define READ  0x0B
#define FIFO  0x0C

// ADXL362 Register Addresses
#define DEVID_AD       0x00
#define DEVID_MST      0x01
#define PARTID         0x02
#define REVID          0x03
#define XDATA          0x08
#define YDATA          0x09
#define ZDATA          0x0A
#define STATUS         0x0B
#define FIFO_ENTRIES_L 0x0C
#define FIFO_ENTRIES_H 0x0D
#define XDATA_L        0x0E
#define XDATA_H        0x0F
#define YDATA_L        0x10
#define YDATA_H        0x11
#define ZDATA_L        0x12
#define ZDATA_H        0x13
#define TEMP_L         0x14
#define TEMP_H         0x15
#define SOFT_RESET     0x1F
#define THRESH_ACT_L   0x20
#define THRESH_ACT_H   0x21
#define TIME_ACT       0x22
#define THRESH_INACT_L 0x23
#define THRESH_INACT_H 0x24
#define TIME_INACT_L   0x25
#define TIME_INACT_H   0x26
#define ACT_INACT_CTL  0x27
#define FIFO_CONTROL   0x28
#define FIFO_SAMPLES   0x29
#define INTMAP1        0x2A
#define INTMAP2        0x2B
#define FILTER_CTL     0x2C
#define POWER_CTL      0x2D
#define SELF_TEST      0x2E

// ACT_INACT_CTL Register settings bitmasks
// NB: most significant two bits are unused
#define DEFAULT_LINK_LOOP  0x00 // 0bxx00 0000
#define LINKED_MODE        0x10 // 0bxx01 0000
#define LOOP_MODE          0x30 // 0bxx11 0000
#define INACT_REF_REF_MODE 0x08 // 0bxx00 1000
#define INACT_REF_ABS_MODE 0x00 // 0bxx00 0000
#define INACT_ENABLE_ON    0x04 // 0bxx00 0100
#define INACT_ENABLE_OFF   0x00 // 0bxx00 0000
#define ACT_REF_REF_MODE   0x02 // 0bxx00 0010
#define ACT_REF_ABS_MODE   0x00 // 0bxx00 0000
#define ACT_ENABLE_ON      0x01 // 0bxx00 0001
#define ACT_ENABLE_OFF     0x00 // 0bxx00 0000

// FIFO_CONTROL Register settings bitmasks
// NB: most significant four bits are unused
#define ABOVE_HALF_ON         0x08 // 0bxxxx 1000
#define ABOVE_HALF_OFF        0x00 // 0bxxxx 0000
#define FIFO_TEMP_ON          0x04 // 0bxxxx 0100
#define FIFO_TEMP_OFF         0x00 // 0bxxxx 0000
#define FIFO_DISABLED         0x00 // 0bxxxx 0000
#define FIFO_OLDEST_SAVE_MODE 0x01 // 0bxxxx 0001
#define FIFO_STREAM_MODE      0x02 // 0bxxxx 0010
#define FIFO_TRIGGERED_MODE   0x03 // 0bxxxx 0011

// INTMAP1 and INTMAP2 Register settings bitmasks
// Use these bitmasks to bind INT1 and INT2 to various states
#define INTX_ACTIVE_LOW   0x80 // 0b1000 0000
#define INTX_AWAKE        0x40 // 0b0100 0000
#define INTX_INACT        0x20 // 0b0010 0000
#define INTX_ACT          0x10 // 0b0001 0000
#define INTX_FIFO_OVERRUN 0x08 // 0b0000 1000
#define INTX_FIFO_WATERMK 0x04 // 0b0000 0100
#define INTX_FIFO_READY   0x02 // 0b0000 0010
#define INTX_DATA_READY   0x01 // 0b0000 0001

// FILTER_CTL Register settings bitmasks
#define RANGE_2G        0x00 // 0b0000 0000
#define RANGE_4G        0x40 // 0b0100 0000
#define RANGE_8G        0x80 // 0b1000 0000
#define HALF_BW_HALF    0x00 // 0b0000 0000
#define HALF_BW_QTR     0x10 // 0b0001 0000
#define EXT_SAMPLE_INT2 0x08 // 0b0000 1000
#define EXT_SAMPLE_OFF  0x00 // 0b0000 0000
#define ODR_12_5_HZ     0x00 // 12.5Hz Output Data Rate 0b0000 0000
#define ODR_25_HZ       0x01 // 25Hz Output Data Rate   0b0000 0001
#define ODR_50_HZ       0x02 // 50Hz Output Data Rate   0b0000 0010
#define ODR_100_HZ      0x03 // 100Hz Output Data Rate  0b0000 0011
#define ODR_200_HZ      0x04 // 200Hz Output Data Rate  0b0000 0100
#define ODR_400_HZ      0x07 // 400Hz Output Data Rate  0b0000 0111

// POWER_CTL Register settings bitmasks
#define EXT_CLK             0x40 // 0b0100 0000 Use clock in INT1 pin 
#define LOW_NOISE_NOM       0x00 // 0b0000 0000
#define LOW_NOISE_LOW       0x10 // 0b0001 0000
#define LOW_NOISE_ULTRA_LOW 0x20 // 0b0010 0000
#define WAKEUP_MODE         0x08 // 0b0000 1000
#define AUTOSLEEP_MODE      0x04 // 0b0000 0100
#define MEASURE_MODE_OFF    0x00 // 0b0000 0000
#define MEASURE_MODE_ON     0x02 // 0b0000 0010

// SELF_TEST Register settings bitmasks
#define SELF_TEST_ON 0x01 // 0b0000 0001

class ADXL362
{
public:

	ADXL362( void );
	ADXL362( int cs_pin );
	ADXL362( int cs_pin, boolean debugSerial );

	/**
	 * The basic initialization and configuration of the ADXL362
	 *
	 * This method sets the pinMode on the SPI CS pin, sets the SPI data mode,
	 * and does a soft reset of the ADXL362.
	 */
	void begin(void);

	/**
	 * Turns on "measure mode" on the ADXL362
	 *
	 * This method enables "measure mode" by setting the bits at mask 0x02 in
	 * register 0x2D.
	 */
	void beginMeasure(void); 

	/**
	 * This method reads only the X-axis data register at the full 12 bits of
	 * resolution
	 *
	 * NB: The four most significant bits of the two bytes are "sign extension"
	 *     bits and are always equal to the most significant bit of the 12 bits
	 *     of X-axis data.
	 *
	 * @param include_sx_bits If true, then the sign extension bits are included
	 *        in the returned value. If false, then the sign extension bits are
	 *        masked out of the returned value.
	 */
	int readXData( int include_sx_bits );

	/**
	 * This method reads only the most significant byte of X-axis data.
	 *
	 * The byte that is read does not have any sign extension bits.
	 */
	byte readXMSB(void);

	/**
	 * This method reads only the Y-axis data register at the full 12 bits of
	 * resolution
	 *
	 * NB: The four most significant bits of the two bytes are "sign extension"
	 *     bits and are always equal to the most significant bit of the 12 bits
	 *     of Y-axis data.
	 *
	 * @param include_sx_bits If true, then the sign extension bits are included
	 *        in the returned value. If false, then the sign extension bits are
	 *        masked out of the returned value.
	 */
	int readYData( int include_sx_bits );

	/**
	 * This method reads only the most significant byte of Y-axis data.
	 *
	 * The byte that is read does not have any sign extension bits.
	 */
	byte readYMSB(void);

	/**
	 * This method reads only the Z-axis data register at the full 12 bits of
	 * resolution
	 *
	 * NB: The four most significant bits of the two bytes are "sign extension"
	 *     bits and are always equal to the most significant bit of the 12 bits
	 *     of Z-axis data.
	 *
	 * @param include_sx_bits If true, then the sign extension bits are included
	 *        in the returned value. If false, then the sign extension bits are
	 *        masked out of the returned value.
	 */
	int readZData( int include_sx_bits );

	/**
	 * This method reads only the most significant byte of Z-axis data.
	 *
	 * The byte that is read does not have any sign extension bits.
	 */
	byte readZMSB(void);

	/**
	 * This method reads all of X,Y,Z, and Temperature in one SPI instruction
	 *
	 * The X,Y, and Z axis data that is read is the full 12 bits with 4 sign
	 * extension bits as the four most significant bits. These four sign
	 * extension bits however are masked to 0 before the data is returned by
	 * reference, so the resulting data is the "true" values of the axis' data.
	 */
	void readXYZTData(int &XData, int &YData, int &ZData, int &Temperature, int include_sx_bits);

	/**
	 * This method reads the temperature data register.
	 *
	 * NB: The returned 16 bit integer has 4 sign extension bits as the four
	 *     most significant bits. These four bits are always the same as the
	 *     most significant bit of actual data.
	 *
	 * @param include_sx_bits If true, then the sign extension bits are included
	 *        in the returned value. If false, then the sign extension bits are
	 *        masked out of the returned value.
	 */
	int readTemp( int include_sx_bits );	

	/**
	 * Set bits in the FLITER_CTL register using the defined bit masks
	 *
	 * Multiple bitmasks may be used by using a bitwise or (|) to combine them.
	 * Refer to the ADXL362 datasheet for a full explanation of the settings.
	 */
	void set_filter_ctl (byte settings_mask);

	/**
	 * Set bits in the POWER_CTL register using the defined bit masks
	 *
	 * Multiple bitmasks may be used by using a bitwise or (|) to combine them.
	 * Refer to the ADXL362 datasheet for a full explanation of the settings.
	 *
	 * NB: Use the ADXL362::beginMeasure method to enable Measure Mode. Use this
	 *     method to set the other bits in this register
	 */
	void set_power_ctl (byte settings_mask);

	/**
	 * Trigger a self test to run
	 *
	 */
	void do_self_test (void);

	/**
	 * Read a single register and return its value
	 *
	 * @param address The adress of the register to read
	 */
	byte read_single_register(byte address);

	/**
	 * Write a single register with a given value
	 *
	 * @param address The address of the register to write
	 * @param value The value to write at the given address
	 */
	void write_single_register(byte address, byte value);

	/**
	 * Read a 16bit word from two consectutive registers
	 *
	 * @param address The address to begin reading at
	 */
	int  read_multiple_registers(byte address);
	
private:
	int _pin;
	boolean _debug;
};

#endif
