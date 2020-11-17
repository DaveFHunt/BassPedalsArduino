// test2.h


#ifndef BassPedals_H_
#define BassPedals_H_
#include <Arduino.h>
#include "ArduinoControl.h"
#include "Menu.h"
#include "Status.h"


#define MenuMode    5			// Interrupt 3 pin 18
#define Reset	 	2			// Interrupt 2 pin 19

#define Up          0
#define Down        1
#define Left        2
#define Right       3

// Pin No's for Lowest Pedal
#define lowPedalPin 28       // Low C
#define referenceBase 24

extern Pedal *pedal[13];

void loop();
void setup();
void startupTone();
void menu();
void statusDisplay(void);
void menuDisplay(void);
void applySettings();

void ButtonReset(void);
void ButtonMenu(void);
void MenuUp(void);
void MenuDown(void);
void MenuLeft(void);
void MenuRight(void);

#endif /* Pedals_H_ */
