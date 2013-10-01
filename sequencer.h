/* 
	
sequencer.cpp library
this library is to be used with standuino fraAngelico hardware
MOZZI library compatible
documentation of the library you find here:
sequencer library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/

#ifndef sequencer_h
#define sequencer_h

#include "Arduino.h"
#include <avr/pgmspace.h>

#define MAX_VALUE 255
#define MIN_VALUE 0

#define MILLIS 0
#define MICROS 1

  
class sequencer
{
  public: 
  
    sequencer(int _timeBase); // constructor 
	void update(long _time);
    void clockIn();
    boolean clockReady();
    unsigned char clockCount(); // return clock
    void setTempo(int _tempo);
    int getTempo();
    void tapIn(long _time);
    void play(); // midiPlayCommand - nachysta a resetuje sequencer a na dalsi clock in zacne hrat od 1. doby
    void stop(); // midiStopCommand - stop - nevysílá stepReady ale posílá clock!
    void midiContinue();  // midiContinueCommand
    void reset(); 
    void playStop(); // flipFlop
    void setNumberOfSteps(unsigned char _numberOfSteps);
    unsigned char getNumberOfSteps();
    void jumpToStep(unsigned char _step);	
	unsigned char getCurrentStep();
	boolean stepReady();
	boolean isPlaying();
	void stepIn();
	void setClockDivider(unsigned char _clockDivider);
	void setGrooveAmount(unsigned char _amt);
	unsigned char getGrooveAmount();
	void setGrooveOn();
	void setGrooveOff();

  private: 
  	
	int timeBase;
	int tempo;
	unsigned char grooveAmt;
	unsigned char stepCounter;
	unsigned char clockCounter;
	unsigned char stepsReady;
	unsigned char clocksReady;
	unsigned char clockDivider;
	unsigned char numberOfSteps;
	unsigned char numberOfTaps;
	
	long quarterNoteTime;
	long tapTime;
	long lastTime;
	long clockLenght(unsigned char _clockCounter,unsigned char _stepCounter);
	long clockLenght();
	
	int getGrooveCompensation(unsigned char _clockCounter,unsigned char _stepCounter);
	boolean grooveOn;
	
	
	boolean playing;
	
	
};

#endif