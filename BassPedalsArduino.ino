// Do not remove the include below
#include "BassPedals.h"
#include "LCD4Bit_mod.h"
#include <WString.h>
#include "Status.h"

//create object to control an LCD.
//number of lines in display=2
LCD4Bit_mod lcd = LCD4Bit_mod(2);

Pedal *pedal[13];
MenuSwitch *menuSwitch[4];
bool menuMode = false;
long debouncing_time = 500; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

//create and load settings object
Status currentStatus;

void setStop(Parameter parm) {
	currentStatus.setStop(parm);
}

void setKey(Parameter parm) {
	currentStatus.setKey(parm);
}

void setHarmony(Parameter parm) {
	currentStatus.setHarmony(parm);
}

void setPortamento(Parameter parm){
currentStatus.setPortamento(parm);
}

void setup() {
//Initialise LCD
	lcd.init();
	lcd.clear();
	lcd.cursorTo(1, 0); //line=1, x=0
	lcd.printIn((char*) "Initialising");

//	Assign LCD to menu
	Menu::SetLCD(&lcd);
	currentStatus.SetLCD(&lcd);
	lcd.cursorTo(2, 0);
	lcd.printIn((char*) "Settings Loaded");

	Pedal::SetBasePitch(referenceBase);
	int pin = lowPedalPin;
	int offset = 0;

// Create pedal instances
	for (int i = 0; i < 13; i++) {
		pedal[i] = new Pedal(pin, offset);
		pin += 2;
		offset++;
	}

//  Set MIDI baud rate:
	Serial.begin(31250);

// Create menu buttons
	menuSwitch[Up] = new MenuSwitch(43, MenuUp);
	menuSwitch[Down] = new MenuSwitch(45, MenuDown);
	menuSwitch[Left] = new MenuSwitch(47, MenuLeft);
	menuSwitch[Right] = new MenuSwitch(49, MenuRight);

// Create Menu entries
	Menu* Root = new Menu();
	Menu* Stop = new Menu("Stop", Root);
	Menu* Key = new Menu("Key", Root);
	Menu* Harm = new Menu("Harmony", Root);
	Menu* Cont = new Menu("Control", Root);
 	Menu* OB = new Menu(Stop, setStop, Parameter("Organ Bass", 16,40));
 	Menu* MB = new Menu(Stop, setStop, Parameter("Modular", 39,2));
 	Menu* SH101 = new Menu(Stop, setStop, Parameter("SH101 1", 39,7));
 	Menu* Rot = new Menu(Stop, setStop, Parameter("Rotary", 18,8));
 	Menu* Church = new Menu(Stop, setStop, Parameter("Church", 19,8));
 	Menu* Orch = new Menu(Stop, setStop, Parameter("Orchestra", 48,9));
 	Menu* Tuba = new Menu(Stop, setStop, Parameter("Tuba", 48));
 	Menu* KG = new Menu(Stop, setStop, Parameter("KG", 39,24));
 	Menu* BG = new Menu(Stop, setStop, Parameter("Bass Gt", 33,4));
 	Menu* Ls = new Menu(Stop, setStop, Parameter("Lead Saw", 81,4));
 	Menu* AB = new Menu(Stop, setStop,  Parameter("Square",  38,24));

 	Menu* Saw = new Menu(Stop, setStop,Parameter("Sawtooth 2",  91));
	Menu* C = new Menu( Key, setKey, Parameter("C", 0));
	Menu* CS = new Menu(Key, setKey, Parameter("C#",1));
	Menu* D = new Menu( Key, setKey,Parameter("D",2));
	Menu* DS = new Menu(Key, setKey, Parameter("D#",3));
	Menu* E = new Menu( Key, setKey,Parameter("E",4));
	Menu* F = new Menu( Key, setKey,Parameter("F", 5));
	Menu* FS = new Menu( Key, setKey,Parameter("F#", 6));
	Menu* G = new Menu( Key, setKey, Parameter("G",7));
	Menu* GS = new Menu( Key, setKey,Parameter("G#", 8));
	Menu* A = new Menu( Key, setKey, Parameter("A",9));
	Menu* AS = new Menu( Key, setKey, Parameter("A#",10));
	Menu* B = new Menu( Key, setKey, Parameter("B",11));
	Menu* Fifth = new Menu( Harm, setHarmony, Parameter("Fifth",0x10));
	Menu* Octave = new Menu( Harm, setHarmony,Parameter("Octave",0x80));
	Menu* None = new Menu( Harm, setHarmony,Parameter("None", 0x00));
	Menu* All = new Menu( Harm, setHarmony, Parameter("All",0x90));
 	Menu* PortOn = new Menu(Cont, setPortamento,Parameter("Port. On", 0xff));
 	Menu* PortOff = new Menu(Cont, setPortamento,Parameter( "Port. Off",0x0));

	// Interrupts do these last
	new MomentarySwitch(18, ButtonMenu, 5);
	new MomentarySwitch(19, ButtonReset, 4);

	applySettings();
	// Startup sound
	startupTone();

	lcd.cursorTo(2, 0);
	lcd.printIn((char*) "Play Mode");
}

void applySettings() {
	currentStatus.Apply();
	currentStatus.Display();
}

void loop() {

	if (!menuMode)
		for (int i = 0; i < 13; i++)
			pedal[i]->Poll();
	else
		menu();
}

void startupTone() {

	for (int note = 0x1E; note < 0x30; note++) {
		//Note on channel 1 (0x90), some note value (note)
		Send(NoteOn, note, 100);
		delay(note * 2);
		//Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
		Send(NoteOn, note, 00);
	}
}

void menu() {

	lcd.clear();
	lcd.cursorTo(1, 0);
	lcd.printIn((char*) "Menu Mode");

	// Go back to where we were
	Menu::Navigate('r');
	while (menuMode) {
		for (int i = 0; i < 4; i++)
			menuSwitch[i]->Poll();
	}
	applySettings();
	lcd.cursorTo(2, 0);
	lcd.printIn((char*) "Settings Applied");

}

void ButtonReset(void) {
	if ((long) (micros() - last_micros) >= debouncing_time * 1000) {
		Send(SystemReset, 0, 0);
		lcd.clear();
		applySettings();
		currentStatus.Display();
		lcd.cursorTo(2, 0);
		lcd.printIn((char*) "Reset         ");
		last_micros = micros();
	}
}

void ButtonMenu(void) {
	if ((long) (micros() - last_micros) >= debouncing_time * 1000) {
		menuMode = !menuMode;
		last_micros = micros();
	}
}

void MenuUp(void) {
	Menu::Navigate('u');
}

void MenuDown(void) {
	Menu::Navigate('d');
}

void MenuLeft(void) {
	Menu::Navigate('l');
}

void MenuRight(void) {
	Menu::Navigate('r');
}
