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

//TODO verify right pin number
#define SW1_pin A0 //PC0
#define SW2_pin A1 //PC1

///time components
PCA85073A rtc;
int timeDateArr[13]; //{seconds_tenths, seconds_ units, minute_tenths, minute_unit, hour_tenths, hour_unit, day_tenths, day_unit, weekday, month_tenths, month_unit, year_tenths, year_units}

unsigned int timeNow = 0;
unsigned int timeLast = 0;

bool awake = true;

void setup() {
	//pin setup
	pinMode(SW1_pin, INPUT_PULLUP);
	pinMode(SW2_pin, INPUT_PULLUP);

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

	rtc.timeDateGet(timeDateArr);

	//attach interupts<
	attachInterrupt(digitalPinToInterrupt(SW1_pin), WakeUpSW1, LOW);
	attachInterrupt(digitalPinToInterrupt(SW2_pin), WakeUpSW2, LOW);
}

// the loop function runs over and over again until power down or reset
void loop() {	
	if (awake) {
		timeNow = millis();



	}
	else {
		LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
	}
}

void displayTime(){

}

void displayDate() {

}


void WakeUpSW1() {
	if (!awake) {
		awake = true;
		timeLast = millis();
	}
}


void WakeUpSW2() {
	if (!awake) {
		awake = true;
		timeLast = millis();
	}

}
