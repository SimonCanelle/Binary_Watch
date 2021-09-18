/*Author : Simon Janelle-Bombardier
* Created : 2021-09-15* 
* 
* I2C driver for PCA85073A from NXP
*
* Register map, All register are 8 bit
* 
  address	|	name	|	bit 7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	|
Control and status registers
00h			  Control_1 |  EXT_TEST	|	-	|  STOP	|	SR  |    -	|   CIE	| 12_24 |CAP_SEL|
01h			  Control_2	|	AIE		|	AF	|	MI	|   HMI	|	TF	|	COF[2:0]			|
02h			  Offset	|	MODE	|	OFFSET[6:0]                                         |
03h			  RAM_byte  |  B[7:0]                                                           |
Time and date registers 
04h			  Seconds	|	OS		|   SECONDS (0 to 59)[6:0]                              |
05h			  Minutes	|	-		|   MINUTES (0 to 59)[6:0]								|
06h			  Hours		|	-		|	-		AMPM | HOURS[4:0] (1 to 12) in 12-hour mode	|
									|	HOURS[5:0] (0 to 23) in 24-hour mode				|
07h			  Days		|	-			-	|	DAYS (1 to 31)								|
08h			  Weekdays	|	-		|	-	|	-	|	-	|	-	|WEEKDAYS[2:0] (0 to 6)	| 
09h			  Months	|	-		|	-	|	-	|	MONTHS[4:0] (1 to 12)				|
0Ah			  Years		|	YEARS[7:4][3:0] (0 to 99)										|
Alarm registers
0Bh			Second_alarm|	AEN_S	|	 SECOND_ALARM[6:0] (0 to 59)						|
0Ch			Minute_alarm|	AEN_M	|	 MINUTE_ALARM[6:0] (0 to 59)						|
0Dh			  Hour_alarm|	AEN_H	|	-	|  AMPM	|HOUR_ALARM[4:0] (1 to 12)in 12-hour mode|
											|	HOUR_ALARM[5:0] (0 to 23) in 24-hour mode	|
0Eh			  Day_alarm |	AEN_D	|	-	|	DAY_ALARM[5:0] (1 to 31)					|
0Fh		  Weekday_alarm |	AEN_W	|	-	|	-	|	-	|	-	|WEEKDAY_ALARM[2:0] (0 to 6)|
Timer registers
10h			Timer_value |	T[7:0]															|
11h			Timer_mode	|	-		|	-	|	-	|	TCF[4:3]	|	TE	|	TIE	| TI_TP |
* 
*/

#ifndef PCA85073A_H
#define PCA85073A_H

#include <Wire.h>

//I2C address
#define I2CAddressRead 0xA2 //10100010
#define I2CAddressWrite 0xA3 //10100011

//register addresses
#define Control1 0x00
#define Control2 0x01
#define Offset 0x02
#define RAM_Byte 0x03
#define Seconds 0x04
#define Minutes 0x05
#define Hours 0x06
#define Days 0x07
#define Weekdays 0x08
#define Months 0x09
#define Years 0x0A
#define Second_alarm 0x0B
#define Minute_alarm 0x0C
#define Hour_alarm 0x0D
#define Day_alarm 0x0E
#define Weekday_alarm 0x0F
#define Timer_value 0x10
#define Timer_mode 0x11

/// <summary>
/// Class for PCA85073A RTC I2C communication
/// </summary>
class PCA85073A
{
	
public:
	//constructor standard
	PCA85073A();
	
	//set Controls 1 and 2
	void setControl1(byte value);
	void setControl2(byte value);
	void setOffset(bool mode, int value);
	
	//r/w time and date
	void timeDateGet(int timeDateArr[] );
	void timeDateSet(int second, int minute, int hour);//set only time
	void timeDateSet(int day, int weekday, int month, int year);//set only date
	void timeDateSet(int second, int minute, int hour, int day, int weekday, int month, int year);//year is only the 2 last digit (ex : for 2025 -> year = 25)
	
	//TODO alarm setup
	//
	//TODO timer setup


private:
	//read write operations on each register
	byte readRegister(int address);
	void readRegister(int address, int numberOfBytes, byte byteArr[]);//read multiple bytes
	void writeRegister(int address, byte b);
	void writeRegister(int address, byte b[]);//write multiple bytes


};

#endif // !PCA85073A_I2C