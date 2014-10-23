/* 
	
sequencer.cpp library
this library is to be used with 
MOZZI library compatible
documentation of the library you find here:
sequencer library

Created by Vaclav Pelousek 2013 www.pelousek.eu
for Standuino wwww.standuino.eu

*/


#include "Arduino.h"
#include <avr/pgmspace.h>
#include "sequencer.h"

#define DEFAULT_CLOCK_DIVIDER 6
#define DEFAULT_NUMBER_OF_STEPS 16
#define DEFAULT_TEMPO 120

PROGMEM prog_uchar groove[]={
  128,192,64,128, 128,192,64,128, 128,192,64,128, 128,192,64,128, };

sequencer::sequencer(int _timeBase){ // 1 microseconds 1000 milliseconds 61 microseconds
	timeBase=_timeBase;
	clockDivider=DEFAULT_CLOCK_DIVIDER;
	numberOfSteps=DEFAULT_NUMBER_OF_STEPS;
	tempo=DEFAULT_TEMPO;
}

long sequencer::clockLenght(unsigned char _clockCounter,unsigned char _stepCounter){

/*
 60 seconds per minute
 60000 milliseconds per minute
 60000000 microseconds per minute
 
 60000*(1000/timebase) per minute
 
 beat = 4 quarter notes = 24 clocks

 tiky za minutu
 jak dlouho trvá jeden tik?
 minuta / počet tiků
 */
 	long grooveNow=getGrooveCompensation(_clockCounter,_stepCounter);
 	if(grooveNow>=fixedClockLength) grooveNow=fixedClockLength-20;
 	else if(grooveNow<=-fixedClockLength) grooveNow=-fixedClockLength+20;
 	return fixedClockLength + grooveNow;
 
}

long sequencer::calculateClockLenght(){
	return ( ( 60000*(1000/timeBase) ) / (tempo * 24) );
}

long sequencer::clockLenght(){
	return fixedClockLength;
}

int sequencer::getGrooveCompensation(unsigned char _clockCounter,unsigned char _stepCounter){
 return int((grooveAmt * (pgm_read_byte_near(groove + _stepCounter%16)-128))/64);
 // groove typ // groove amount
}

void sequencer::setGrooveAmount(unsigned char _amt){
	grooveAmt=_amt;
}
unsigned char sequencer::getGrooveAmount(){
	return grooveAmt;
}
void sequencer::setGrooveOn(){
	grooveOn=true;
}
void sequencer::setGrooveOff(){
	grooveOn=false;
}
	
void  sequencer::update(long _time){
		
	if(grooveOn){
	int _clockL=clockLenght(clockCounter,stepCounter);
		while(_time-lastTime>=_clockL) {
			clocksReady++;
			lastTime+=_clockL;
//			lastTime=_time;
		}
	}
	
	else{
	int _clockL=clockLenght();
		while((_time-lastTime)>=_clockL) { //clockLenght()
			clocksReady++;
			lastTime+=_clockL;
			//lastTime=_time;
		}		
	}
	
}

void  sequencer::clockIn(){
	clockCounter++; 
	if(clockCounter>=clockDivider) clockCounter=0, stepsReady++;
}

boolean  sequencer::clockReady(){
	boolean _ready = false;
	if(clocksReady>0) clocksReady--,  _ready = true;
	return _ready;
}
unsigned char sequencer::clockCount(){ // return clock
 return clockCounter;
}   
void sequencer::setClockDivider(unsigned char _clockDivider){
	clockDivider=_clockDivider;
}
void sequencer::setTempo(int _tempo){
	tempo=_tempo;
	if(tempo==0) tempo=1;
	fixedClockLength=calculateClockLenght();
}
int sequencer::getTempo(){
	return tempo;
}

void sequencer::tapIn(long _time){

	if( (_time-tapTime) >= 2000*(1000/timeBase) ){
		tapTime=_time, numberOfTaps=0, quarterNoteTime=0;
	}
	
	else{
		if( (_time-tapTime) < clockLenght()*clockDivider*4 ){
		///???
			clocksReady=0;
			stepsReady=1;
			stepCounter+=stepCounter%4;
			lastTime=_time;
		} 
		
	//lastTime=_time;
	 //time circle buffer
		quarterNoteTime+=_time-tapTime;
		numberOfTaps++;
		tempo=( 60000*(1000/timeBase) ) / (quarterNoteTime/numberOfTaps) ; 
		tapTime=_time;
	}
	
}

void sequencer::play(){ // midiPlayCommand - nachysta a resetuje sequencer a na dalsi clock in zacne hrat od 1. doby
	playing=true;
	stepCounter=-1;
	clockCounter=clockDivider-1;
	stepsReady=0;
}

void sequencer::stop(){ // midiStopCommand - stop - nevysílá stepReady ale posílá clock!
	playing=false;
}
boolean sequencer::isPlaying(){ 
	return playing;
}
void sequencer::midiContinue(){  // midiContinueCommand
	playing=true;
}
void sequencer::reset(){

}
void sequencer::playStop(){ // flipFlop
	if(playing) stop();
	else play();
}
void sequencer::setNumberOfSteps(unsigned char _numberOfSteps){
 numberOfSteps=_numberOfSteps;
}

unsigned char sequencer::getNumberOfSteps(){
	return numberOfSteps;
}

void sequencer::jumpToStep(unsigned char _step){
	stepCounter=_step;
	//stepsReady=1;
}

unsigned char sequencer::getCurrentStep(){
 return stepCounter;
}
void  sequencer::stepIn(){
	stepCounter++; 
	if(stepCounter>=numberOfSteps) stepCounter=0;
}

boolean sequencer::stepReady(){

	boolean _ready = false;
	if(!playing) stepsReady=0;
	if(stepsReady>0) stepsReady--,_ready = true;
	
	return _ready;
}

