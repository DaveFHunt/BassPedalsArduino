/*
 * DHSwitch.cpp
 *
 *  Created on: 15 Nov 2012
 *      Author: Huntcorp
 */

#include "Arduino.h"
#include "ArduinoControl.h"

int Pedal::sBasePitch = 0;
int Pedal::sHarmony = 0;

byte oldCmd = 0;
void Send(byte cmd, byte data1, byte data2, byte channel) {
	// We will use running status
	if (cmd != oldCmd) {
		oldCmd = cmd;
		byte status;
		status = cmd | (channel & 0x0F);
		Serial.write(status);
	}
	data1 &= 0x7F;
	Serial.write(data1);

	if (cmd < ProgramChange) {
		data2 &= 0x7F;
		Serial.write(data2);

	}
}

void Pedal::setPitch() {
	mPitch[0] = sBasePitch + mPitchOffset;
	if (sHarmony && 0x10)
		mPitch[1] = mPitch[0] + 7;
	else
		mPitch[1]=0;
	if (sHarmony && 0x80)
		mPitch[2] = mPitch[0] + 12;
	else
		mPitch[2]=0;
}

ArduinoControl::ArduinoControl(int pinNo) {
	mPinNo = pinNo;
	mCurrentValue = LOW;
	pinMode(mPinNo, INPUT);
	digitalWrite(pinNo, LOW);
}


Pedal::Pedal(int pinNo, int pitchOffset) :
		ArduinoControl(pinNo) {
	mPitchOffset = pitchOffset;
	setPitch();
}

void Pedal::Poll() {
	int newValue = digitalRead(mPinNo);
	if (mCurrentValue != newValue) {
		if (newValue == HIGH) {
			Send(NoteOn, mPitch[0], 100);
			if (mPitch[1])
				Send(NoteOn, mPitch[1], 100);
			if (mPitch[2])
				Send(NoteOn, mPitch[2], 100);
		} else {
			Send(NoteOff, mPitch[0], 00);
			if (mPitch[1])
				Send(NoteOff, mPitch[1], 00);
			if (mPitch[2])
				Send(NoteOff, mPitch[2], 00);
		}
		mCurrentValue = newValue;
	}
}



MomentarySwitch::MomentarySwitch(int pinNo, void (*isr)(void),
		int interuptNo) :
		ArduinoControl(pinNo) {

	attachInterrupt(interuptNo, isr, RISING);
}

MenuSwitch::MenuSwitch(int pinNo, void (*MenuFunction)(void)) :
		ArduinoControl(pinNo) {
	mMenuFunction = MenuFunction;
}
;
void MenuSwitch::Poll() {
	int newValue = digitalRead(mPinNo);
	if (mCurrentValue != newValue) {
		if (newValue == HIGH)
			mMenuFunction();
		mCurrentValue = newValue;
	}

}
