/*
 Name:		Binary_Watch.ino
 Created:	15/09/2021 12:49:16
 Author:	Simon Janelle-Bombardier
*/

// the setup function runs once when you press reset or power the board

#include <Wire.h>
#include "PCA85073A.h"
#include "src/Low-Power-master/LowPower.h" //source : https://github.com/rocketscream/Low-Power

#define RAM_CODE 0b01010101 //if this value is registered in RTC ram byte, RTC is configured

//TODO : verify right pin number
#define SW1_pin A0 //PC0  : time
#define SW2_pin A1 //PC1  : date
#define col0 4 //D4
#define col1 5 //D5
#define col2 6 //D6
#define col3 7 //D7
#define row0 3 //D3
#define row1 2 //D2
#define row2 9 //D9
#define row3 8 //D8


///time components
PCA85073A rtc;
int timeDateArr[13]; //{seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday, month_tenths, month_unit, year_tenths, year_units}

unsigned int timeNow = 0;
unsigned int timeLast = 0;
unsigned int displayDelay = 5000; //in ms

bool awake = true;
bool DorT = false; // false : SW1, true : SW2
bool date = false; //false : show month/day,  true : show year
bool timeChange = false;
bool dateChange = false;

/* led positionning in led matrix[col][row]
3    x     x
2    x  x  x
1 x  x  x  x
0 x  x  x  x
  0  1  2  3
*/

bool ledMatrix[4][4];

void setup() {
	//switches
	pinMode(SW1_pin, INPUT_PULLUP);
	pinMode(SW2_pin, INPUT_PULLUP);
	//led col
	pinMode(col0, OUTPUT);
	pinMode(col1, OUTPUT);
	pinMode(col2, OUTPUT);
	pinMode(col3, OUTPUT);
	//led row
	pinMode(row0, OUTPUT);
	pinMode(row1, OUTPUT);
	pinMode(row2, OUTPUT);
	pinMode(row3, OUTPUT);

	//TODO : put all unconnected pin in input pull-up mode

	//verify RTC RAM
	byte b = rtc.getRAMByte();
	if (b != RAM_CODE) {
		b = 0b00000000; //TODO : set right settings
		rtc.setControl1(b);

		b = 0b00000000; //TODO : set right settings
		rtc.setControl2(b);

		b = 0b00000000; //TODO : set right settings
		rtc.setOffset(0, b);

		rtc.setRAMByte(RAM_CODE);
	}	

	//attach interupts
	attachInterrupt(digitalPinToInterrupt(SW1_pin), WakeUpSW1, LOW);
	attachInterrupt(digitalPinToInterrupt(SW2_pin), WakeUpSW2, LOW);
}

// the loop function runs over and over again until power down or reset
void loop() {	
	//normal display
	if (awake && !(timeChange|dateChange)) {
		timeNow = millis();
		if (timeNow - timeLast >= displayDelay) {
			awake = false;
		}

		rtc.timeDateGet(timeDateArr);

		//input the wanted data in matrix to be displayed
		if (DorT) {
			if (date) {
				//input year in led matrix
				valueToMatrix(2, 0, timeDateArr[11], timeDateArr[12]);
			}
			else {
				//input month/day in led matrix
				valueToMatrix(timeDateArr[9], timeDateArr[10], timeDateArr[6], timeDateArr[7]);
			}
		}
		
		else {
			//input minutes and hour in led matrix
			valueToMatrix(timeDateArr[2], timeDateArr[3], timeDateArr[4], timeDateArr[5]);
		}		
		//display matrix
		display();

	}

	//modify time
	else if (awake && timeChange) {
		//TODO : set time
	}

	//modify date
	else if (awake && dateChange) {
		//TODO : set Date
	}

	//go to sleep
	else {
		stopDisplay();
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	}
}

void display(){
	//columns
	if(ledMatrix[0][0]| ledMatrix[0][1]) digitalWrite(col0, LOW);
	else digitalWrite(col0, HIGH);

	if (ledMatrix[1][0] | ledMatrix[1][1] | ledMatrix[1][2] | ledMatrix[1][3]) digitalWrite(col1, LOW);
	else digitalWrite(col1, HIGH);

	if (ledMatrix[2][0] | ledMatrix[2][1] | ledMatrix[2][2]) digitalWrite(col2, LOW);
	else digitalWrite(col2, HIGH);

	if (ledMatrix[3][0] | ledMatrix[3][1] | ledMatrix[3][2] | ledMatrix[3][3]) digitalWrite(col3, LOW);
	else digitalWrite(col3, HIGH);

	//rows
	if (ledMatrix[0][0] | ledMatrix[1][0] | ledMatrix[2][0] | ledMatrix[3][0])digitalWrite(row0, HIGH);
	else digitalWrite(row0, LOW);

	if (ledMatrix[0][1] | ledMatrix[1][1] | ledMatrix[2][1] | ledMatrix[3][1])digitalWrite(row1, HIGH);
	else digitalWrite(row1, LOW);

	if (ledMatrix[1][2] | ledMatrix[2][2] | ledMatrix[3][2])digitalWrite(row2, HIGH);
	else digitalWrite(row2, LOW);

	if (ledMatrix[1][3] | ledMatrix[3][3])digitalWrite(row2, HIGH);
	else digitalWrite(row2, LOW);

}

void stopDisplay() {	
	digitalWrite(col0, HIGH);
	digitalWrite(col1, HIGH);
	digitalWrite(col2, HIGH);
	digitalWrite(col3, HIGH);
	digitalWrite(row0, LOW);
	digitalWrite(row1, LOW);
	digitalWrite(row2, LOW);
	digitalWrite(row3, LOW);
}

void valueToMatrix(int num1tenth, int num1unit, int num2tenth, int num2unit) {
	//first tenths
	ledMatrix[0][0] = num1tenth & 0b00000001;
	ledMatrix[0][1] = (num1tenth & 0b00000010) >> 1;

	//first units
	ledMatrix[1][0] = num1unit & 0b00000001;
	ledMatrix[1][1] = (num1unit & 0b00000010) >> 1;
	ledMatrix[1][2] = (num1unit & 0b00000100) >> 2;
	ledMatrix[1][3] = (num1unit & 0b00001000) >> 3;

	//second tenths
	ledMatrix[2][0] = num2tenth & 0b00000001;
	ledMatrix[2][1] = (num2tenth & 0b00000010) >> 1;
	ledMatrix[2][2] = (num2tenth & 0b00000100) >> 2;

	//second units
	ledMatrix[3][0] = num2unit & 0b00000001;
	ledMatrix[3][1] = (num2unit & 0b00000010) >> 1;
	ledMatrix[3][2] = (num2unit & 0b00000100) >> 2;
	ledMatrix[3][3] = (num2unit & 0b00001000) >> 3;
}


void WakeUpSW1() {
	if (!awake) {
		awake = true;
		timeLast = millis();
		DorT = false;
	}
	else {
		timeLast = millis();//reset display time
		DorT = false;
	}
}


void WakeUpSW2() {
	if (!awake) {
		awake = true;
		timeLast = millis();
		date = false;
		DorT = true;
	}
	else {
		date = !date;
		timeLast = millis();//reset display time
		DorT = true;
	}

}
