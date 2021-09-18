#include "PCA85073A.h"
#include <Wire.h>


/// <summary>
/// Constructor for the PCA85073A I2C driver
/// </summary>
PCA85073A::PCA85073A () {
	Wire.begin();
}

/// <summary>
/// 
/// </summary>
/// <param name="value"></param>
void PCA85073A::setControl1(byte value) {

}

/// <summary>
/// 
/// </summary>
/// <param name="value"></param>
void PCA85073A::setControl2(byte value) {

}

/// <summary>
/// 
/// </summary>
/// <param name="mode"></param>
/// <param name="value"></param>
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
/// 
/// </summary>
/// <param name="timeDateArr"></param>
void PCA85073A::timeDateGet(int timeDateArr[]) { //{second, minute, hour, day, weekday, month, year}

}

/// <summary>
/// 
/// </summary>
/// <param name="second"></param>
/// <param name="minute"></param>
/// <param name="hour"></param>
/// <param name="day"></param>
/// <param name="weekday"></param>
/// <param name="month"></param>
/// <param name="year"></param>
void PCA85073A::timeDateSet(int second, int minute, int hour, int day, int weekday, int month, int year){//year is only the 2 last digit (ex : for 2025 -> year = 25)

}

/// <summary>
/// 
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
byte PCA85073A::readRegister(int address) {

}

/// <summary>
/// 
/// </summary>
/// <param name="address"></param>
void PCA85073A::writeRegister(int address) {

}


