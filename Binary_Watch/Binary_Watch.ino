/*
 Name:		Binary_Watch.ino
 Created:	15/09/2021 12:49:16
 Author:	Simon Janelle-Bombardier
*/

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

enum State {
	minutes,
	hours,
	month,
	day,
	year
}state;


///time components
PCA85073A rtc;
int timeDateArr[13]; //{seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday, month_tenths, month_unit, year_tenths, year_units}
					 
//wake timer
unsigned int timeWNow = 0;
unsigned int timeWLast = 0;
const unsigned int displayDelay = 5000; //in ms
//debounce timer
unsigned int timeDNow = 0;
unsigned int timeDLast = 0;
const unsigned int debounceDelay = 100; //in ms
const unsigned int timeSetupDelay = 5000; //in ms
//loop timer
unsigned int timeLNow = 0;
unsigned int timeLLast = 0;
const unsigned int loopDelay = 1000; //in ms

//buttons press counter
int SW1Press = 0;
int SW2Press = 0;

//states
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

/// <summary>
/// setup is executed once at start of MCU
/// </summary>
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
	//apply settings if not already set up
	if (b != RAM_CODE) {
		b = 0b00000101; 
		rtc.setControl1(b);

		b = 0b00000111; 
		rtc.setControl2(b);

		b = 0b00000011; // value calculated from datasheet
		rtc.setOffset(0, b);

		rtc.setRAMByte(RAM_CODE);
	}	

	//attach interupts
	attachInterrupt(digitalPinToInterrupt(SW1_pin), WakeUpSW1, LOW);
	attachInterrupt(digitalPinToInterrupt(SW2_pin), WakeUpSW2, LOW);
}

/// <summary>
/// loop is executed in loop after setup 
/// </summary>
void loop() {	
	//normal display
	if (awake && !(timeChange||dateChange)) {
		timeWNow = millis();
		timeLNow = timeWNow;

		if (timeWNow - timeWLast >= displayDelay) {
			awake = false;
		}

		//update display every s
		if (timeLNow - timeLLast >= loopDelay) {
			timeLLast = timeLNow;

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
	}

	//modify time
	else if (awake && timeChange) {
		int change = SW1Press - SW2Press;

		switch (state)
		{
		case minutes:
			int m = timeDateArr[2] * 10 + timeDateArr[3];
			m += change;
			//wrap around
			if (m < 0) {
				m += 60;
			}
			else if (m >= 60) {
				m -= 60;
			}

			timeDateArr[2] = m / 10;
			timeDateArr[3] = m % 10;
			break;

		case hours:
			int m = timeDateArr[4] * 10 + timeDateArr[5];
			m += change;
			//wrap around
			if (m < 0) {
				m += 24;
			}
			else if (m >= 24) {
				m -= 24;
			}

			timeDateArr[4] = m / 10;
			timeDateArr[5] = m % 10;

			break;

		case month:
			int m = timeDateArr[9] * 10 + timeDateArr[10];
			m += change;
			//wrap around
			if (m < 0) {
				m += 12;
			}
			else if (m >= 12) {
				m -= 12;
			}

			timeDateArr[9] = m / 10;
			timeDateArr[10] = m % 10;

			break;

		case day://TODO : implement right number of day for the month
			int m = timeDateArr[6] * 10 + timeDateArr[7];
			m += change;
			//wrap around
			if (m < 0) {
				m += 31;
			}
			else if (m >= 31) {
				m -= 31;
			}

			timeDateArr[6] = m / 10;
			timeDateArr[7] = m % 10;
			break;

		case year:
			int m = timeDateArr[11] * 10 + timeDateArr[12];
			m += change;
			//wrap around
			if (m < 0) {
				m += 31;
			}
			else if (m >= 31) {
				m -= 31;
			}

			timeDateArr[11] = m / 10;
			timeDateArr[12] = m % 10;
			break;
		}
		
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
	}

	//go to sleep
	else {
		stopDisplay();
		//change interupt function
		detachInterrupt(digitalPinToInterrupt(SW1_pin));
		detachInterrupt(digitalPinToInterrupt(SW2_pin));
		attachInterrupt(digitalPinToInterrupt(SW1_pin), WakeUpSW1, LOW);
		attachInterrupt(digitalPinToInterrupt(SW2_pin), WakeUpSW1, LOW);

		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	}
}

/// <summary>
/// setup the input/output for led display from led matrix
/// </summary>
void display(){
	//columns
	if(ledMatrix[0][0] | ledMatrix[0][1]) digitalWrite(col0, LOW);
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

/// <summary>
/// turn all led off
/// </summary>
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

/// <summary>
/// transfer the values to the led matrix
/// </summary>
/// <param name="num1tenth">digit in col 1</param>
/// <param name="num1unit">digit in col 2</param>
/// <param name="num2tenth">digit in col 3</param>
/// <param name="num2unit">digit in col 4</param>
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

/// <summary>
/// interupt function of the time button
/// </summary>
void WakeUpSW1() {
		awake = true;
		timeWLast = millis();
		DorT = false;	
		detachInterrupt(digitalPinToInterrupt(SW1_pin));
		attachInterrupt(digitalPinToInterrupt(SW1_pin), ClickSW1, CHANGE);
		
}

/// <summary>
/// 
/// </summary>
void ClickSW1() {
	timeDNow = millis();
	if (timeDNow - timeDLast >= debounceDelay) {
		timeDLast = timeDNow;

		if (digitalRead(SW1_pin) == LOW) {
		DorT = false;
		
		}
		else {
		timeWLast = millis();
			if (timeDNow - timeDLast >= timeSetupDelay) {
				if (!timeChange) {
					timeChange = true;
					DorT = false;
					date = false;
					state = State::minutes;
					SW1Press = 0;
					SW2Press = 0;
				}
				else {
					timeChange = false;

					int m = timeDateArr[2]*10 + timeDateArr[3];
					int h = timeDateArr[4] * 10 + timeDateArr[5];
					int M = timeDateArr[9] * 10 + timeDateArr[10];
					int d = timeDateArr[6] * 10 + timeDateArr[7];
					int y = timeDateArr[11] * 10 + timeDateArr[12];
					rtc.timeDateSet(0,m,h,false,d,0,M,y);
				}
			}
			else if(timeChange)SW1Press++;
		}
	}
}

/// <summary>
/// interupt function of the date function
/// </summary>
void WakeUpSW2() {
		awake = true;
		timeWLast = millis();
		date = false;
		DorT = true;	
		detachInterrupt(digitalPinToInterrupt(SW1_pin));
		attachInterrupt(digitalPinToInterrupt(SW1_pin), ClickSW1, CHANGE);
}

/// <summary>
/// 
/// </summary>
void ClickSW2() {
	timeDNow = millis();
	if (timeDNow - timeDLast >= debounceDelay) {
		timeDLast = timeDNow;

		if (digitalRead(SW1_pin) == LOW) {
			DorT = false;

		}
		else {
			timeWLast = millis();
			if (timeDNow - timeDLast >= timeSetupDelay && timeChange) {
				switch (state)
				{
				case minutes:
					state = State::hours;
					DorT = false;
					break;
				case hours:
					state = State::month;
					DorT = true;
					date = false;
					break;
				case month:
					state = State::day;
					DorT = true;
					date = false;
					break;
				case day:
					state = State::year;
					DorT = true;
					date = true;
					break;
				case year:
					state = State::minutes;
					DorT = false;
					break;
				}
			}
			else if (timeChange)SW2Press++;
		}
	}
}
