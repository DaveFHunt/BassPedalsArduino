/*
 * DHSwitch.h
 *
 *  Created on: 18 Nov 2012
 *      Author: Huntcorp
 */

#ifndef DHSWITCH_H_ 
#define DHSWITCH_H_
typedef uint8_t byte;
enum MIDIType {
	NoteOff = 0x80, ///< Note Off
	NoteOn = 0x90, ///< Note On
	AfterTouchPoly = 0xA0, ///< Polyphonic AfterTouch
	ControlChange = 0xB0, ///< Control Change / Channel Mode
	ProgramChange = 0xC0, ///< Program Change
	AfterTouchChannel = 0xD0, ///< Channel (monophonic) AfterTouch
	PitchBend = 0xE0, ///< Pitch Bend
	SystemExclusive = 0xF0, ///< System Exclusive
	TimeCodeQuarterFrame = 0xF1, ///< System Common - MIDI Time Code Quarter Frame
	SongPosition = 0xF2, ///< System Common - Song Position Pointer
	SongSelect = 0xF3, ///< System Common - Song Select
	TuneRequest = 0xF6, ///< System Common - Tune Request
	Clock = 0xF8, ///< System Real Time - Timing Clock
	Start = 0xFA, ///< System Real Time - Start
	Continue = 0xFB, ///< System Real Time - Continue
	Stop = 0xFC, ///< System Real Time - Stop
	ActiveSensing = 0xFE, ///< System Real Time - Active Sensing
	SystemReset = 0xFF, ///< System Real Time - System Reset
	InvalidType = 0x00 ///< For notifying errors
};

// helper function
void Send(byte cmd, byte data1, byte data2, byte channel = 0x00);

class ArduinoControl {
public:
	ArduinoControl(int pinNo);

protected:
	int mPinNo;
	bool mCurrentValue;
};

class Pedal: public ArduinoControl {
public:
	Pedal(int pinNo, int pitchOffset);

	inline static void SetBasePitch(int basePitch) {
		sBasePitch = basePitch;
	}

	inline static int GetBasePitch() {
		return sBasePitch;
	}

	void Poll();
	void setPitch();
	static void setHarmony(int harmony){sHarmony=harmony;}
private:
	int mPitch[3]; // MIDI note number assigned to pedal.
	int mPitchOffset; // Offset from the global base Pitch

	static int sBasePitch;
	static int sHarmony;
};

class MomentarySwitch: public ArduinoControl {
public:
	MomentarySwitch(int pinNo, void (*isr)(void), int interuptNo);
private:
};

class MenuSwitch: public ArduinoControl {
public:
	MenuSwitch(int pinNo, void (*MenuFunction)(void));
	void Poll();
private:
	void (*mMenuFunction)(void);
};

#endif /* DHSWITCH_H_ */
