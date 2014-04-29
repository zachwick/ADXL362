/*
 Arduino Library for Analog Devices ADXL362 - Micropower 3-axis accelerometer
 go to http://www.analog.com/ADXL362 for datasheet
 
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2012
 by Anne Mahaffey - hosted on http://annem.github.com/ADXL362
 
 */ 

#include "Arduino.h"

#ifndef ADXL362_h
#define ADXL362_h


class ADXL362
{
public:

	ADXL362();
		
	/**
	 * The basic initialization and configuration of the ADXL362
	 *
	 * This method sets the pinMode on the SPI CS pin, sets the SPI data mode,
	 * and does a soft reset of the ADXL362.
	 */
	void begin();

	/**
	 * Turns on "measure mode" on the ADXL362
	 *
	 * This method enables "measure mode" by setting the bits at mask 0x02 in
	 * register 0x2D.
	 */
	void beginMeasure(); 

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
	byte readXMSB();

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
	byte readYMSB();

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
	byte readZMSB();

	/**
	 * This method reads all of X,Y,Z, and Temperature in one SPI instruction
	 *
	 * The X,Y, and Z axis data that is read is the full 12 bits with 4 sign
	 * extension bits as the four most significant bits. These four sign
	 * extension bits however are masked to 0 before the data is returned by
	 * reference, so the resulting data is the "true" values of the axis' data.
	 */
	void readXYZTData(int XData, int YData, int ZData, int Temperature);

	/**
	 * This method reads the temperature data register.
	 *
	 * NB: The returned 16 bit integer has 4 sign extension bits as the four
	 *     most significant bits. These four bits are always the same as the
	 *     most significant bit of actual data.
	 */
	int readTemp();


	//
	// Activity/Inactivity interrupt functions
	//
	void setupDCActivityInterrupt(int threshold, byte time);	
	void setupDCInactivityInterrupt(int threshold, int time);
    void setupACActivityInterrupt(int threshold, byte time);
	void setupACInactivityInterrupt(int threshold, int time);
	
	// need to add the following functions
	// void mapINT1(
	// void mapINT2
	// void autoSleep
	// void activityInterruptControl
	//		-Activity, Inactivity, Both
	//		- Referenced, Absolute
	//		- Free Fall, Linked Mode, Loop Mode
	
	
	void checkAllControlRegs();
	
	
	//  Low-level SPI control, to simplify overall coding
	byte SPIreadOneRegister(byte regAddress);
	void SPIwriteOneRegister(byte regAddress, byte regValue);
	int  SPIreadTwoRegisters(byte regAddress);
	void SPIwriteTwoRegisters(byte regAddress, int twoRegValue);

	
private:

};

#endif
