/*
 * menuroot.cpp
 *
 *  Created on: 27 Feb 2013
 *      Author: huntcorp
 */

#include "Menu.h"

Menu* Menu::sRoot = 0;
Menu* Menu::sCurrentPosition = 0;
LCD4Bit_mod* Menu::sLCD=0;

void Menu::Init(void) {
	mPrevSibling = 0;
	mNextSibling = 0;
	mFirstChild = 0;
	mLastChild = 0;
	mMenuFunction = 0;
	mParent = 0;
}

void Menu::Display() {

#ifdef CPLUSMODE
	cout<<mName<<endl;
#else
	sLCD->clear();
	sLCD->cursorTo(1, 0);
	sLCD->printIn(mParent->mParm.getName());
	sLCD->cursorTo(2, 0);
	sLCD->printIn((char*)" >");
	sLCD->cursorTo(2, 2);
	sLCD->printIn(mParm.getName());
#endif
}

Menu::Menu(_STRINGCLASS name, Menu* parent)
:mParm (name,0){
	Init();
	mParent = parent;
	mParent->addChild(this);
}

Menu::Menu(Menu* parent, void (*function)(Parameter), Parameter parm)
:mParm(parm){
	Init();
	mParent = parent;
	mParent->addChild(this);
	mMenuFunction = function;
}
Menu::Menu()
    :mParm("",0)   {
	Init();
	sRoot = this;
	sCurrentPosition = this;
	mParent = 0;
	mPrevSibling =this;
	mNextSibling=this;
}

void Menu::addChild(Menu *newChild) {

	newChild->mPrevSibling = mLastChild;
	newChild->mNextSibling = mFirstChild;

	if (!mFirstChild) {
		mFirstChild = newChild;
		mLastChild = newChild;
	} else
		mLastChild->mNextSibling = newChild;

//the children are a double linked list; so we need this to link them correctly
	mFirstChild->mPrevSibling = newChild;
	mLastChild = newChild;
}
void Menu::List() {
	Display();

	if (mFirstChild)
		mFirstChild->List();

	if ((mNextSibling) && mNextSibling != this
			&& mNextSibling != mParent->mFirstChild)
		mNextSibling->List();
}
void Menu::Navigate(char direction) {
	switch (direction) {
	case 'u':
		sCurrentPosition->Up();
		break;
	case 'd':
		sCurrentPosition->Down();
		break; //execution starts at this case label
	case 'l':
		sCurrentPosition->Left();
		break;
	case 'r':
		sCurrentPosition->Right();
		break;
	default:
		break;
	}
	sCurrentPosition->Display();
}
;

void Menu::Up() {
	sCurrentPosition = mPrevSibling;
}

void Menu::Down() {
	sCurrentPosition = mNextSibling;
}

void Menu::Right() {

	if (mMenuFunction or !mFirstChild) {
		mMenuFunction(mParm);
	} else
		sCurrentPosition = mFirstChild;
}

void Menu::Left() {
	if (mParent)
		sCurrentPosition = mParent;

}
