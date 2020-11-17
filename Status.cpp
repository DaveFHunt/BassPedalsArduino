/*
 * Status.cpp
 *
 *  Created on: 1 May 2013
 *      Author: huntcorp
 */

#include "EEPROMAnything.h"
#include "Basspedals.h"
typedef uint8_t byte;



Parameter::Parameter(const String txt, const int val,const int val2 ) {
	mValue = val;
	mValue2=val2;
	txt.toCharArray(mText, 16);
}

Status::Status() {
	int start = 0;
	EEPROM_readAnything(start, *this);
}

void Status::Save() {
	int start = 0;
	EEPROM_writeAnything(start, *this);
	mLCD->cursorTo(2, 0);
	mLCD->printIn((char*) "Settings Applied");
}

void Status::Display() {

	mLCD->clear();
	mLCD->cursorTo(1, 0);
	mStop.getName();
	mLCD->printIn(mStop.getName());
	mLCD->cursorTo(1, 8);
	mLCD->printIn(" ");
	mLCD->cursorTo(1, 9);
	mLCD->printIn(mKey.getName());
	mLCD->cursorTo(1, 12);
	mLCD->printIn(mHarmony.getName());
}

void Status::Apply() {
	Pedal::setHarmony(mHarmony.getValue());
	int pitch = referenceBase + mKey.getValue();
	Pedal::SetBasePitch(pitch);
	for (int i = 0; i < 13; i++)
		pedal[i]->setPitch();

	// make sure we are using the correct instrument map
	Send(ControlChange, 32, 0, 0);

	if (mPortamento.getValue()) {
		Send(ControlChange, 0x41, 0xff, 0);
		Send(ControlChange, 0x05, 300, 0);
		Send(ControlChange, 0x54, pitch, 0 );
	} else
		Send(ControlChange, 0x41, 0, 0);

	Send(ControlChange, 0, mStop.getValue2(), 0);
	Send(ProgramChange, mStop.getValue(), 0, 0);
	Status::Save();

}
