/*
 * menuroot.h
 *
 *  Created on: 27 Feb 2013
 *  Author: huntcorp
 */

#ifndef MENUROOT_H_
#define MENUROOT_H_


#ifdef CPLUSMODE
#define _STRINGCLASS string
#include <string>
#include <iostream>
using namespace std;
#else
#include "WString.h"
#include "LCD4Bit_mod.h"

#define _STRINGCLASS String
#endif
#include "Status.h"


//
// we could have  separate derived classes for the root items and leaves
// this is too much like hard work we have one class and 3 constructors
// the top level of our menu is a single item
//


class Menu {
public:
	Menu(Menu* parent, void (*function)(Parameter),Parameter);   // leaf constructor
	Menu(_STRINGCLASS name,Menu*);                               // node constructor
	Menu();													     // root constructor
	static void Navigate(char direction);
	void List();
    static void SetLCD(LCD4Bit_mod* LCD) {sLCD=LCD;}
private:
	static Menu* sRoot;
	static Menu* sCurrentPosition;
	static LCD4Bit_mod* sLCD;


	Parameter mParm;

	Menu* mFirstChild;
	Menu* mLastChild;
	Menu* mPrevSibling;
	Menu* mNextSibling;
	Menu* mParent;

	void addChild(Menu *child);

	void Display();
	void Init(void);
	void (*mMenuFunction)(Parameter);
	void Up();
	void Down();
	void Right();
	void Left();
};


#endif /* MENUROOT_H_ */

