/**
 * ADXL362_SimpleRead_Sign_Extension_Bits.ino
 * Simple XYZ axis reading example for Analog Devices ADXL362
 * Micropower 3-axis accelerometer using (and not using) the
 * sign extension bits.
 * Go to http://www.analog.com/ADXL362 for datasheet
 *
 * License: CC BY-SA
 * Copyright 2014 zachwick <zach@zachwick.com>
 *
 * Connect SCLK, MISO, MOSI, and CSB of ADXL362 to
 * SCLK, MISO, MOSI, and DP 10 of Arduino 
 * (check http://arduino.cc/en/Reference/SPI for details)
 *
 */ 

#include <SPI.h>
#include <ADXL362.h>

ADXL362 xl;

void setup(){
    Serial.begin(9600);
    xl.begin();                   // Setup SPI protocol, issue device soft reset
    xl.beginMeasure();            // Switch ADXL362 to measure mode  
    xl.checkAllControlRegs();     // Burst Read all Control Registers, to check for proper setup
	
    Serial.print("\n\nBegin Loop Function:\n");
}

void loop(){
    /*
      The "sign extension" bits are the most significant four bits.
      These bits are always the same as the most significant bit of the
      12bits of data per axis.
      Leaving the sign extension bits in the returned value is prone to
      data misinterpretation.
    */
  
    // X axis, Y axis, Z axis, and Temp data without the sign extension bits.
    int X,Y,Z,T;
    // "Raw" X axis, Y axis, Z axis, and Temp data with the sign extension bits intact.    
    int sx_X, sx_Y, sx_Z, sx_T;
    
    // Read the X-axis register and mask out the sign extension bits.
    X = xl.readXData(0);
    // Read the X-axis register and keep the sign extension bits present
    sx_X = xl.readXData(1);

    // Read the Y-axis register and mask out the sign extension bits.
    Y = xl.readYData(0);
    // Read the Y-axis register and keep the sign extension bits present
    sx_Y = xl.readYData(1);

    // Read the Z-axis register and mask out the sign extension bits.
    Z = xl.readZData(0);	 
    // Read the Z-axis register and keep the sign extension bits present
    sx_Z = xl.readZData(1);
    
    // Reat the Temperature register and mask out the sign extension bits.
    T = xl.readTemp(0);
    // Read the Temperature register and keep the sign extension bits present
    sx_T = xl.readTemp(1);

    Serial.print("\n");    
    delay(100);                // Arbitrary delay to make serial monitor easier to observe
}

