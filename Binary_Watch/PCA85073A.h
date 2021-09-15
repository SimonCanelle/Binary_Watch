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

#ifndef PCA85073A_I2C





class PCA85073A
{
public:
	//constructor standard
	//constructor custom I2C address
	//
	//some text to do a test
	//
	//
	//

private:
	//read write operations on each register


};

#endif // !PCA85073A_I2C