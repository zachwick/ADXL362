/**
 * ADXL362_BurstRead.ino
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

ADXL362 xl = ADXL362(4);

void setup(){
    Serial.begin(9600);
    xl.begin();                   // Setup SPI protocol, issue device soft reset
    Serial.println(String(RANGE_2G | HALF_BW_QTR | ODR_12_5_HZ,HEX));
    xl.set_filter_ctl (RANGE_2G | HALF_BW_QTR | ODR_12_5_HZ);
    xl.set_power_ctl (LOW_NOISE_ULTRA_LOW);
    xl.beginMeasure();            // Switch ADXL362 to measure mode
    
    Serial.println(String(xl.read_single_register(FILTER_CTL),HEX));
    Serial.println(String(xl.read_single_register(POWER_CTL),HEX));
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
  
    // X, Y, and Z axis data without the sign extension bits.
    int X,Y,Z, Temp;
    // "Raw" X, Y, and Z axis data with the sign extension bits intact.    
    int sx_X, sx_Y, sx_Z, sx_Temp;
    
    // Read all data registers in a burst and remove the sign extension
    // bits. Read in a burst to ensure that all measurments correspond
    // to the same sample time
    xl.readXYZTData(X,Y,Z,Temp,0);
    
    Serial.print("X: ");
    Serial.print(String(X,HEX));
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(String(Y,HEX));
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.print(String(Z,HEX));
    Serial.print(" ");
    Serial.print("T: ");
    Serial.print(String(Temp,HEX));
    Serial.print("\n");
    // Read all data registers and include the sign extension bits
    //xl.readXYZTData(X,Y,Z,Temp,1);
    
    Serial.print("\n");    
    delay(60);                // Arbitrary delay to make serial monitor easier to observe
}

