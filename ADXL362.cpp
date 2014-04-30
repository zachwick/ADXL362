/*
 Arduino Library for Analog Devices ADXL362 - Micropower 3-axis accelerometer
 go to http://www.analog.com/ADXL362 for datasheet
 
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created June 2012
 by Anne Mahaffey - hosted on http://annem.github.com/ADXL362
 
 */ 

#include <Arduino.h>
#include <ADXL362.h>
#include <SPI.h>

const int slaveSelectPin = 4;
const bool debugSerial = 1;

ADXL362::ADXL362() {

}


//
//  begin()
//  Initial SPI setup, soft reset of device
//
void ADXL362::begin() {
  pinMode(slaveSelectPin, OUTPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);	//CPHA = CPOL = 0    MODE = 0
  SPI.setClockDivider(SPI_CLOCK_DIV4); // Set the SPI bus to operate at 4MHz
  delay(1000);
    
  // soft reset
  SPIwriteOneRegister(0x1F, 0x52);  // Write to SOFT RESET, "R"
  delay(10);
  Serial.println("Soft Reset\n");
 }

 
//
//  beginMeasure()
//  turn on Measurement mode - required after reset
// 
void ADXL362::beginMeasure() {
  byte temp = SPIreadOneRegister(0x2D);	// read Reg 2D before modifying for measure mode
  if (debugSerial) {Serial.print(  "Setting Measeurement Mode - Reg 2D before = "); Serial.print(temp); }

  // turn on measurement mode
  byte tempwrite = temp | 0x02;			// turn on measurement bit in Reg 2D
  SPIwriteOneRegister(0x2D, tempwrite); // Write to POWER_CTL_REG, Measurement Mode
  delay(10);	
  
  if (debugSerial) {
  temp = SPIreadOneRegister(0x2D);
  Serial.print(  ", Reg 2D after = "); Serial.println(temp); Serial.println();}
}


//
//  readXData(), readYData(), readZData(), readTemp()
//  Read X, Y, Z, and Temp registers
//
int ADXL362::readXData(int include_sx_bits){
  int XDATA = SPIreadTwoRegisters(0x0E);

  if (!include_sx_bits) {
	  int sx_mask = 0xF000;
	  XDATA &= (~sx_mask);
  }

  if (debugSerial) {
	  Serial.print(  "XDATA = "); 
	  Serial.print(String(XDATA, HEX));
	  Serial.print("\n");
  }

  return XDATA;
}

// Read only the most significant byte of X-axis data
byte ADXL362::readXMSB() {
	int XDATA = SPIreadOneRegister(0x08);
	return XDATA;
}

int ADXL362::readYData(int include_sx_bits){
  int YDATA = SPIreadTwoRegisters(0x10);

  if (!include_sx_bits) {
	  int sx_mask = 0xF000;
	  YDATA &= (~sx_mask);
  }

  if (debugSerial) {
	  Serial.print(  "YDATA = ");
	  Serial.print(String(YDATA, HEX));
	  Serial.print("\n");
  }

  return YDATA;
}

// Read only the most significant byte of Y-axis data
byte ADXL362::readYMSB() {
	int YDATA = SPIreadOneRegister(0x09);
	return YDATA;
}

int ADXL362::readZData(int include_sx_bits){
  int ZDATA = SPIreadTwoRegisters(0x12);

  if (!include_sx_bits) {
	  int sx_mask = 0xF000;
	  ZDATA &= (~sx_mask);
  }

  if (debugSerial) {
	  Serial.print("ZDATA = ");
	  Serial.print(String(ZDATA, HEX));
	  Serial.print("\n");
  }

  return ZDATA;
}

// Read only the most significant byte of Z-axis data
byte ADXL362::readZMSB() {
	int ZDATA = SPIreadOneRegister(0x0A);
	return ZDATA;
}

int ADXL362::readTemp(int include_sx_bits){
  int TEMP = SPIreadTwoRegisters(0x14);

  if (!include_sx_bits) {
	  int sx_mask = 0xF000;
	  TEMP &= (~sx_mask);
  }

  if (debugSerial) {
	  Serial.print("\tTEMP = "); 
	  Serial.print(TEMP); 
	  Serial.print("\n");
  }

  return TEMP;
}

void ADXL362::readXYZTData(int &XData, int &YData, int &ZData, int &Temperature, int include_sx_bits){
  	byte x_l, x_h, y_l, y_h, z_l, z_h;
	byte temp_l, temp_h;
	// The actual X,Y,Z axis data is only 12 bits. The remaining 4 bits are "sign
	// extension" bits and are always equal to the most significant bit.
	// They are masked to zero so as to return the correct value back by reference.
	byte sx_mask = 0xF0;

	// burst SPI read
	// A burst read of all three axis is required to guarantee all measurements correspond to same sample time
	digitalWrite(slaveSelectPin, LOW);
	SPI.transfer(0x0B);  // read instruction
	SPI.transfer(0x0E);  // Start at XData Reg

	x_l = SPI.transfer(0x00);
	x_h = SPI.transfer(0x00);
	if (!include_sx_bits) {
		x_h &= (~sx_mask);
	}
	XData = x_l + (x_h << 8);

	y_l = SPI.transfer(0x00);
	y_h = SPI.transfer(0x00);
	if (!include_sx_bits) {
		y_h &= (~sx_mask);
	}
	YData = y_l + (y_h <<  8);

	z_l = SPI.transfer(0x00);
	z_h = SPI.transfer(0x00);
	if (!include_sx_bits) {
		z_h &= (~sx_mask);
	}
	ZData = z_l + (z_h << 8);

	temp_l = SPI.transfer(0x00);
	temp_h = SPI.transfer(0x00);
	if (!include_sx_bits) {
		temp_h &= (~sx_mask);
	}
	Temperature = temp_l + (temp_h << 8);

	digitalWrite(slaveSelectPin, HIGH);
  
	if (debugSerial) {
		Serial.print("x_l: "); Serial.print(String(x_l, HEX)); Serial.print(" ");
		Serial.print("x_h: "); Serial.print(String(x_h, HEX)); Serial.print(" "); Serial.print("\n");
		Serial.print("y_l: "); Serial.print(String(y_l, HEX)); Serial.print(" ");
		Serial.print("y_h: "); Serial.print(String(y_h, HEX)); Serial.print(" "); Serial.print("\n");
		Serial.print("z_l: "); Serial.print(String(z_l, HEX)); Serial.print(" ");
		Serial.print("z_h: "); Serial.print(String(z_h, HEX)); Serial.print(" "); Serial.print("\n");
		Serial.print("temp_l: "); Serial.print(String(temp_l, HEX)); Serial.print(" ");
		Serial.print("temp_h: "); Serial.print(String(temp_h, HEX)); Serial.print(" "); Serial.print("\n");
	}
}



void ADXL362::setupDCActivityInterrupt(int threshold, byte time){
  //  Setup motion and time thresholds
  SPIwriteTwoRegisters(0x20, threshold);
  SPIwriteOneRegister(0x22, time);

  // turn on activity interrupt
  byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);  // Read current reg value
  ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x01);     // turn on bit 1, ACT_EN  
  SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
  ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);       // Verify properly written

  if (debugSerial) {
  Serial.print("DC Activity Threshold set to ");  	Serial.print(SPIreadTwoRegisters(0x20));
  Serial.print(", Time threshold set to ");  		Serial.print(SPIreadOneRegister(0x22)); 
  Serial.print(", ACT_INACT_CTL Register is ");  	Serial.println(ACT_INACT_CTL_Reg, HEX);
  }
}

void ADXL362::setupACActivityInterrupt(int threshold, byte time){
  //  Setup motion and time thresholds
  SPIwriteTwoRegisters(0x20, threshold);
  SPIwriteOneRegister(0x22, time);
  
  // turn on activity interrupt
  byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);  // Read current reg value
  ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x03);     // turn on bit 2 and 1, ACT_AC_DCB, ACT_EN  
  SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);       // Write new reg value 
  ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);       // Verify properly written

 if (debugSerial) {  
  Serial.print("AC Activity Threshold set to ");  	Serial.print(SPIreadTwoRegisters(0x20));
  Serial.print(", Time Activity set to ");  		Serial.print(SPIreadOneRegister(0x22));  
  Serial.print(", ACT_INACT_CTL Register is ");  Serial.println(ACT_INACT_CTL_Reg, HEX);
  }
}

void ADXL362::setupDCInactivityInterrupt(int threshold, int time){
  // Setup motion and time thresholds
  SPIwriteTwoRegisters(0x23, threshold);
  SPIwriteTwoRegisters(0x25, time);

  // turn on inactivity interrupt
  byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);   // Read current reg value 
  ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x04);      // turn on bit 3, INACT_EN  
  SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
  ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);        // Verify properly written

 if (debugSerial) {
  Serial.print("DC Inactivity Threshold set to ");  Serial.print(SPIreadTwoRegisters(0x23));
  Serial.print(", Time Inactivity set to ");  Serial.print(SPIreadTwoRegisters(0x25));
  Serial.print(", ACT_INACT_CTL Register is ");  Serial.println(ACT_INACT_CTL_Reg, HEX);
  }
}


void ADXL362::setupACInactivityInterrupt(int threshold, int time){
  //  Setup motion and time thresholds
  SPIwriteTwoRegisters(0x23, threshold);
  SPIwriteTwoRegisters(0x25, time);
 
  // turn on inactivity interrupt
  byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);   // Read current reg value
  ACT_INACT_CTL_Reg = ACT_INACT_CTL_Reg | (0x0C);      // turn on bit 3 and 4, INACT_AC_DCB, INACT_EN  
  SPIwriteOneRegister(0x27, ACT_INACT_CTL_Reg);        // Write new reg value 
  ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);        // Verify properly written

 if (debugSerial) {
  Serial.print("AC Inactivity Threshold set to ");  Serial.print(SPIreadTwoRegisters(0x23));
  Serial.print(", Time Inactivity set to ");  Serial.print(SPIreadTwoRegisters(0x25)); 
  Serial.print(", ACT_INACT_CTL Register is ");  Serial.println(ACT_INACT_CTL_Reg, HEX);
  }
}


void ADXL362::checkAllControlRegs(){
	//byte filterCntlReg = SPIreadOneRegister(0x2C);
	//byte ODR = filterCntlReg & 0x07;  Serial.print("ODR = ");  Serial.println(ODR, HEX);
	//byte ACT_INACT_CTL_Reg = SPIreadOneRegister(0x27);      Serial.print("ACT_INACT_CTL_Reg = "); Serial.println(ACT_INACT_CTL_Reg, HEX);
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(0x20);  // Start burst read at Reg 20
  Serial.println("Start Burst Read of all Control Regs - Library version 6-24-2012:");
  Serial.print("Reg 20 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 21 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 22 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 23 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 24 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 25 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 26 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 27 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 28 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 29 = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 2A = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 2B = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 2C = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 2D = "); 	Serial.println(SPI.transfer(0x00), HEX);
  Serial.print("Reg 2E = "); 	Serial.println(SPI.transfer(0x00), HEX);
  
  digitalWrite(slaveSelectPin, HIGH);
}



// Basic SPI routines to simplify code
// read and write one register

byte ADXL362::SPIreadOneRegister(byte regAddress){
  byte regValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);
  regValue = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  return regValue;
}

void ADXL362::SPIwriteOneRegister(byte regAddress, byte regValue){
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);
  SPI.transfer(regValue);
  digitalWrite(slaveSelectPin, HIGH);
}

int ADXL362::SPIreadTwoRegisters(byte regAddress){
  int twoRegValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);  
  twoRegValue = SPI.transfer(0x00);
  twoRegValue = twoRegValue + (SPI.transfer(0x00) << 8);
  digitalWrite(slaveSelectPin, HIGH);

  return twoRegValue;
}  
void ADXL362::SPIwriteTwoRegisters(byte regAddress, int twoRegValue){
  
  byte twoRegValueH = twoRegValue >> 8;
  byte twoRegValueL = twoRegValue;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);  
  SPI.transfer(twoRegValueL);
  SPI.transfer(twoRegValueH);
  digitalWrite(slaveSelectPin, HIGH);
}
