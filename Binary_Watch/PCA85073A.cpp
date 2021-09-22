/*	
*	Name : PCA85073A.cpp
*	Author : Simon Janelle-Bombardier
*	Created : 2021-09-15
*/

#include "PCA85073A.h"
#include <Wire.h>

//////////////////////
///Public Functions///
//////////////////////

/// <summary>
/// Constructor for the PCA85073A I2C driver
/// </summary>
PCA85073A::PCA85073A () {
	Wire.begin();

	//verify clock mode
	if((readRegister(Control1)&0b00000010) == 2) {
		mode24 = false;
	}
	else {
		mode24 = true;
	}
}

/// <summary>
/// Set the register Control_1 Value
///  bit	|	name	|	default value
///	  7		|  EXT_TEST |	0
///   6		|	-		|	0
///   5		|	STOP	|	0
///   4		|	SR		|	0
///   3		|	-		|	0
///   2		|	CIE		|	0 
///   1		|	12_24	|	0
///   0		|	CAP_SEL	|	0
/// </summary>
/// <param name="value">value to input in register</param>
void PCA85073A::setControl1(byte value) {
	writeRegister(Control1, value);

	if ((value & 0b00000010) == 2) {
		mode24 = false;
	}	else {
		mode24 = true;
	}
}

/// <summary>
/// Set the register Control_2 Value
///  bit	|	name	|	default value
///	  7		|   AIE		|	0
///   6		|	AF		|	0
///   5		|	MI		|	0
///   4		|	HMI		|	0
///   3		|	TF		|	0
///   2		|	COF[2]	|	0 
///   1		|	COF[1]	|	0
///   0		|	COF[0]	|	0
/// </summary>
/// <param name="value">value to input in register</param>
void PCA85073A::setControl2(byte value) {
	writeRegister(Control2, value);
}

/// <summary>
/// Set values and mode of the CIE
/// </summary>
/// <param name="mode">0 : once every two hours, 1 : once every 4 seconds</param>
/// <param name="value">value of the correction parameter</param>
void PCA85073A::setOffset(bool mode, int value) {
	byte  b = mode << 7 & (0b01111111 & value);
	writeRegister(Offset, b);
}

/// <summary>
/// Get the value in the RAM byte
/// </summary>
/// <returns>value of the register</returns>
byte PCA85073A::getRAMByte() {
	return readRegister(RAM_Byte);
}

/// <summary>
/// set the value in the RAM byte
/// </summary>
/// <param name="b">value to input</param>
void PCA85073A::setRAMByte(byte b) {
	writeRegister(RAM_Byte, b);
}

/*Recommended method for reading the time:
	1. Send a START condition and the slave address(see Table 38 on page 34) for write
	(A2h)
		2. Set the address pointer to 4 (Seconds)by sending 04h
		3. Send a RESTART condition or STOP followed by START
		4. Send the slave address for read(A3h)
		5. Read Seconds
		6. Read Minutes
		7. Read Hours
		8. Read Days
		9. Read Weekdays
		10. Read Months
		11. Read Years
		12. Send a STOP condition
*/
/// <summary>
/// Get an array of the time and date separated into digits
/// </summary>
/// <param name="timeDateArr">
/// Array needs to have a length of 14
/// {seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday_tenths, weekday_unit, month_tenths, month_unit, year_tenths, year_units}/// 
/// </param>
void PCA85073A::timeDateGet(int timeDateArr[]) { //{seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday, month_tenths, month_unit, year_tenths, year_units}
	if (sizeof(timeDateArr) != 14) return;

	byte b[7];
	readRegister(Seconds, 7, b);

	//seconds
	timeDateArr[0] = (b[0] & 0b01110000) >> 4;
	timeDateArr[1] = b[0] & 0b00001111;

	//minutes
	timeDateArr[2] = (b[1] & 0b01110000) >> 4;
	timeDateArr[3] = b[1] & 0b00001111;
	
	//hours
	if (mode24) {
		timeDateArr[4] = (b[2] & 0b00110000) >> 4;
		timeDateArr[5] = b[2] & 0b00001111;
	}
	else {
		timeDateArr[4] = (b[2] & 0b00010000) >> 4;
		timeDateArr[5] = b[2] & 0b00001111;
	}

	//days
	timeDateArr[6] = (b[3] & 0b00110000) >> 4;
	timeDateArr[7] = b[3] & 0b00001111;

	//weekdays
	timeDateArr[8] = (b[4] & 0b00000111);

	//months
	timeDateArr[9] = (b[5] & 0b00010000) >> 4;
	timeDateArr[10] = b[5] & 0b00001111;

	//years
	timeDateArr[11] = (b[6] & 0b11110000) >> 4;
	timeDateArr[12] = b[6] & 0b00001111;

}



/// <summary>
/// Set the time and date of the rtc
/// </summary>
/// <param name="second">0 to 59</param>
/// <param name="minute">0 to 59</param>
/// <param name="hour"> 1 to 24</param>
/// <param name="AmPm"> used in 12h mode, true for Pm</param>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
void PCA85073A::timeDateSet(int second, int minute, int hour, bool AmPm, int day, int weekday, int month, int year){//year is only the 2 last digit (ex : for 2025 -> year = 25)
	
	//array of bytes to send
	byte arr[7];

	//masks are applied to register with unused/reserved bits
	//seconds, stored in BCD format 
	arr[0] = (second % 10 | (second / 10) << 4) & 0b01111111; 
	
	//minutes, stored in BCD format
	arr[1] = (minute % 10 | (minute / 10) << 4) & 0b01111111;

	//hours, stored in BCD format
	if (mode24) arr[2] = (hour % 10 | (hour / 10) << 4) & 0b00111111;
	else arr[2] = (AmPm << 5 | hour % 10 | (hour / 10) << 4) & 0b00111111;

	//days, stored in BCD format
	arr[3] = (day % 10 | (day / 10) << 4) & 0b00111111;

	//weekdays
	arr[4] = weekday & 0b00000111;

	//months, stored in BCD format
	arr[5] = (month % 10 | (month / 10) << 4) & 0b00011111;

	//year, stored in BCD format
	arr[6] = year % 10 | (year / 10) << 4;

	writeRegister(Seconds, 7, arr);
}

/// <summary>
/// Set the time of the rtc
/// </summary>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
/// <param name="AmPm"> used in 12h mode, true for Pm</param>
void PCA85073A::timeDateSet(int second, int minute, int hour, bool AmPm) {//year is only the 2 last digit (ex : for 2025 -> year = 25)
	//array of bytes to send
	byte arr[3];

	//masks are applied to register with unused/reserved bits
	//seconds, stored in BCD format 
	arr[0] = (second % 10 | (second / 10) << 4) & 0b01111111;

	//minutes, stored in BCD format
	arr[1] = (minute % 10 | (minute / 10) << 4) & 0b01111111;

	//hours, stored in BCD format
	if (mode24) arr[2] = (hour % 10 | (hour / 10) << 4) & 0b00111111;
	else arr[2] = (AmPm << 5 | hour % 10 | (hour / 10) << 4) & 0b00111111;

	writeRegister(Seconds, 3, arr);
}

/// <summary>
/// Set the date of the rtc
/// </summary>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
void PCA85073A::timeDateSet(int day, int weekday, int month, int year) {//year is only the 2 last digit (ex : for 2025 -> year = 25)
	//array of bytes to send
	byte arr[4];

	//days, stored in BCD format
	arr[0] = (day % 10 | (day / 10) << 4) & 0b00111111;

	//weekdays
	arr[1] = weekday & 0b00000111;

	//months, stored in BCD format
	arr[2] = (month % 10 | (month / 10) << 4) & 0b00011111;

	//year, stored in BCD format
	arr[3] = year % 10 | (year / 10) << 4;

	writeRegister(Days, 4, arr);
}

///////////////////////
///Private Functions///
///////////////////////

/// <summary>
/// Read the register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <returns>value of register</returns>
byte PCA85073A::readRegister(int address) {
	Wire.beginTransmission(I2CAddress);
	Wire.write(address);
	Wire.endTransmission();	
	
	Wire.requestFrom(I2CAddress,1);
	while (!Wire.available());
	byte b = Wire.read();
	Wire.endTransmission();

	return 	b;
}

/// <summary>
/// Read the register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <param name="numberOfBytes">number of bytes to read</param>
/// <param name="byteArr[]"></param>
/// <returns>value of register</returns>
void PCA85073A::readRegister(int startingAddress, int numberOfBytes, byte byteArr[]) {
	Wire.beginTransmission(I2CAddress);
	Wire.write(startingAddress);
	Wire.endTransmission();

	Wire.requestFrom(I2CAddress, numberOfBytes);
	while (Wire.available() < numberOfBytes);
	for(int i = 0; i< numberOfBytes; i++)byteArr[i] = Wire.read();	
	Wire.endTransmission();
}

/// <summary>
/// Write register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <param name="b">8 bit value to input in register</param>
void PCA85073A::writeRegister(int address, byte b) {
	Wire.beginTransmission(I2CAddress);
	Wire.write(address);
	Wire.write(b);
	Wire.endTransmission();
}

/// <summary>
/// Write register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <param name="numberOfBytes">must be equal or smaller than byte array length</param>
/// <param name="b">8 bit values to input in register</param>
void PCA85073A::writeRegister(int startingAddress, int numberOfBytes, byte b[]) {
	Wire.beginTransmission(I2CAddress);
	Wire.write(startingAddress);
	for (int i = 0; i < numberOfBytes; i++)Wire.write(b[i]);
	Wire.endTransmission();
}


