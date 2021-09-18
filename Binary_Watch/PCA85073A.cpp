#include "PCA85073A.h"
#include <Wire.h>


/// <summary>
/// Constructor for the PCA85073A I2C driver
/// </summary>
PCA85073A::PCA85073A () {
	Wire.begin();
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

}

/// <summary>
/// Set the register Control_2 Value
///  bit	|	name	|	default value
///	  7		|   AIE		|	0
///   6		|	AF		|	0
///   5		|	MI		|	0
///   4		|	HMI		|	0
///   3		|	TF		|	0
///   2		|  COF[2:0]	|	0 
///   1		|			|	0
///   0		|			|	0
/// </summary>
/// <param name="value">value to input in register</param>
void PCA85073A::setControl2(byte value) {

}

/// <summary>
/// Set values and mode of the CIE
/// </summary>
/// <param name="mode">0 : once every two hours, 1 : once every 4 seconds</param>
/// <param name="value">value of the correction parameter</param>
void PCA85073A::setOffset(bool mode, int value) {

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
void PCA85073A::timeDateGet(int timeDateArr[]) { //{seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday_tenths, weekday_unit, month_tenths, month_unit, year_tenths, year_units}
	if (sizeof(timeDateArr) != 14) return;

	
}



/// <summary>
/// Set the time and date of the rtc
/// </summary>
/// <param name="second">0 to 59</param>
/// <param name="minute">0 to 59</param>
/// <param name="hour"> 1 to 24</param>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
void PCA85073A::timeDateSet(int second, int minute, int hour, int day, int weekday, int month, int year){//year is only the 2 last digit (ex : for 2025 -> year = 25)

}

/// <summary>
/// Set the time of the rtc
/// </summary>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
void PCA85073A::timeDateSet(int second, int minute, int hour) {//year is only the 2 last digit (ex : for 2025 -> year = 25)

}

/// <summary>
/// Set the date of the rtc
/// </summary>
/// <param name="day">1 to 31</param>
/// <param name="weekday">1 to 7 (starts Sunday)</param>
/// <param name="month">1 to 12</param>
/// <param name="year">last two digits</param>
void PCA85073A::timeDateSet(int day, int weekday, int month, int year) {//year is only the 2 last digit (ex : for 2025 -> year = 25)

}

/// <summary>
/// Read the register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <returns>value of register</returns>
byte PCA85073A::readRegister(int address) {
	Wire.beginTransmission(I2CAddressWrite);
	Wire.write(address);
	Wire.endTransmission();	
	
	Wire.beginTransmission(I2CAddressRead);
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
	Wire.beginTransmission(I2CAddressWrite);
	Wire.write(startingAddress);
	Wire.endTransmission();

	Wire.beginTransmission(I2CAddressRead);
	for(int i = 0; i< numberOfBytes; i++)byteArr[i] = Wire.read();	
	Wire.endTransmission();
}

/// <summary>
/// Write register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <param name="b">8 bit value to input in register</param>
void PCA85073A::writeRegister(int address, byte b) {

}

/// <summary>
/// Write register at wanted address
/// </summary>
/// <param name="address">from 0x00 to 0x11</param>
/// <param name="b">8 bit values to input in register</param>
void PCA85073A::writeRegister(int address, byte b[]) {

}


