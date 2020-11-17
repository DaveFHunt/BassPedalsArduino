/*
 * Status.h
 *
 *  Created on: 1 May 2013
 *      Author: huntcorp
 */

#ifndef STATUS_H_
#define STATUS_H_
#include "arduino.h"
#include "LCD4Bit_mod.h"
#include "arduinoControl.h"



class Parameter{
public:
  	Parameter(){};
	Parameter(const String txt,const int val, const int val2=0);
  	inline int getValue(void){return mValue;}
  	inline int getValue2(void){return mValue2;}
  	char* getName(){return mText;}
private:
	 char mText[16];
     int  mValue;
     int mValue2;
};

class Status {
public:
	Status();
	void Save();
	void Display();
	void Apply();

	void SetLCD(LCD4Bit_mod* LCD) {mLCD=LCD;}
	void setStop(Parameter parm){mStop=parm;}
	void setKey(Parameter parm){mKey=parm;}
	void setHarmony(Parameter parm){mHarmony=parm;}
	void setBank(Parameter parm){mBank=parm;}
	void setSustain(Parameter parm){mSustain=parm;}
	void setPortamento(Parameter parm){mPortamento=parm;}
private:
	Parameter mStop;
	Parameter mHarmony;
	Parameter mKey;
	Parameter mBank;
	Parameter mSustain;
	Parameter mPortamento;
	LCD4Bit_mod* mLCD;
};

#endif /* STATUS_H_ */
