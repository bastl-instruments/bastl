/*
 * simpleSerialDecoder.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: dasvaclav
 */

#include <simpleSerialDecoder.h>
#include <simpleSerialProtocol.h>
#include <Arduino.h>

#define CLOCK 1
#define STEP 2
#define GATE 3
#define TRIGGER 4
#define START 5
#define STOP 6
#define RESTART 7

#define CHANNEL_TRIGGER 10
#define CHANNEL_GATE 20
#define CHANNEL_CV 30

#define CHANNEL_INTERPOLATE_FROM 40
#define CHANNEL_INTERPOLATE_TO 50
#define CHANNEL_INTERPOLATE 60

#define CHANNEL_MODE 70

#define CHANNEL_VALUE 100


simpleSerialProtocol _serial;
/*
simpleSerialDecoder::simpleSerialDecoder(){

}
*/
void simpleSerialDecoder::init(uint16_t baudrate){
	_serial.init(baudrate);
}

void simpleSerialDecoder::sendClock(uint8_t _number=0){
	_serial.transferMessage(CLOCK,_number);
}
void simpleSerialDecoder::sendStep(uint8_t _number=0){
	_serial.transferMessage(STEP,_number);
}
void simpleSerialDecoder::sendGate(uint8_t _number=0){
	_serial.transferMessage(GATE,_number);
}

void simpleSerialDecoder::sendTrigger(uint8_t _number=0){
	_serial.transferMessage(TRIGGER,_number);
}
void simpleSerialDecoder::sendStart(uint8_t _number=0){
	_serial.transferMessage(START,_number);
}
void simpleSerialDecoder::sendStop(uint8_t _number=0){
	_serial.transferMessage(STOP,_number);
}
void simpleSerialDecoder::sendRestart(uint8_t _number=0){
	_serial.transferMessage(RESTART,_number);
}

void simpleSerialDecoder::sendChannelTrigger(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_TRIGGER+_channel,_number);
}
void simpleSerialDecoder::sendChannelGate(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_GATE+_channel,_number);
}
void simpleSerialDecoder::sendChannelCV(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_CV+_channel,_number);
}
void simpleSerialDecoder::sendChannelInterpolateFrom(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_INTERPOLATE_FROM+_channel,_number);
}
void simpleSerialDecoder::sendChannelInterpolateTo(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_INTERPOLATE_TO+_channel,_number);
}
void simpleSerialDecoder::sendChannelInterpolate(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_INTERPOLATE+_channel,_number);
}
void simpleSerialDecoder::sendChannelMode(uint8_t _channel, uint8_t _number){
	_serial.transferMessage(CHANNEL_MODE+_channel,_number);
}
void simpleSerialDecoder::sendChannelValue(uint8_t _channel, uint8_t _value, uint8_t _number){
	_serial.transferMessage(CHANNEL_VALUE+(_value*10)+_channel,_number);
}


void simpleSerialDecoder::update(){
	if(_serial.recieveMessage()){
	//	Serial.println("rcv");
		uint8_t _type=_serial.getMessageType();
		uint8_t _value=_serial.getMessageValue();
		switch(_type){
		case CLOCK :
			if(clockInCallback!=0){
				clockInCallback(_value);
			}
			break;
		case STEP :
			if(stepCallback!=0){
				stepCallback(_value);
			}
			break;
		case GATE:
			if(gateCallback!=0){
				gateCallback(_value);
			}
		break;
		case TRIGGER:
			if(triggerCallback!=0){
				triggerCallback(_value);
			}
			break;
		case START:
			if(startCallback!=0){
				startCallback(_value);
			}
			break;
		case STOP:
			if(stopCallback!=0){
				stopCallback(_value);
			}
			break;
		case RESTART:
			if(restartCallback!=0){
				restartCallback(_value);
			}
			break;
		default:
			if(_type>=CHANNEL_TRIGGER && _type < CHANNEL_TRIGGER+10){
				if(channelTriggerCallback!=0){
					channelTriggerCallback(_type-CHANNEL_TRIGGER,_value);
				}
			}
			else if(_type>=CHANNEL_GATE && _type < CHANNEL_GATE+10){
				if(channelGateCallback!=0){
					channelGateCallback(_type-CHANNEL_GATE,_value);
				}
			}
			else if(_type>=CHANNEL_INTERPOLATE_FROM && _type < CHANNEL_INTERPOLATE_FROM+10){
				if(channelInterpolateFromCallback!=0){
					channelInterpolateFromCallback(_type-CHANNEL_INTERPOLATE_FROM,_value);
				}
			}
			else if(_type>=CHANNEL_INTERPOLATE_TO && _type < CHANNEL_INTERPOLATE_TO+10){
				if(channelInterpolateToCallback!=0){
					channelInterpolateToCallback(_type-CHANNEL_INTERPOLATE_TO,_value);
				}
			}
			else if(_type>=CHANNEL_INTERPOLATE && _type < CHANNEL_INTERPOLATE+10){
				if(channelInterpolateCallback!=0){
					channelInterpolateCallback(_type-CHANNEL_INTERPOLATE,_value);
				}
			}
			else if(_type>=CHANNEL_MODE && _type < CHANNEL_MODE+10){
				if(channelModeCallback!=0){
					channelModeCallback(_type-CHANNEL_MODE,_value);
				}
			}
			else if(_type>=CHANNEL_VALUE){
				if(channelValueCallback!=0){
					channelValueCallback((_type-CHANNEL_VALUE)%10,(_type-CHANNEL_VALUE)/10,_value);
				}
			}
			break;


		}
	}
}





void simpleSerialDecoder::attachClockCallback(void(*clockInCallback)(uint8_t number)){
	this->clockInCallback = clockInCallback;
}
void simpleSerialDecoder::attachStepCallback(void(*stepCallback)(uint8_t number)){
	this->stepCallback = stepCallback;
}
void simpleSerialDecoder::attachGateCallback(void(*gateCallback)(uint8_t number)){
	this->gateCallback = gateCallback;
}
void simpleSerialDecoder::attachTriggerCallback(void(*triggerCallback)(uint8_t number)){
	this->triggerCallback = triggerCallback;
}
void simpleSerialDecoder::attachStartCallback(void(*startCallback)(uint8_t number)){
	this->startCallback = startCallback;
}
void simpleSerialDecoder::attachStopCallback(void(*stopCallback)(uint8_t number)){
	this->stopCallback = stopCallback;
}
void simpleSerialDecoder::attachRestartCallback(void(*restartCallback)(uint8_t number)){
	this->restartCallback = restartCallback;
}
void simpleSerialDecoder::attachChannelTriggerCallback(void(*channelTriggerCallback)(uint8_t channel, uint8_t number)){
	this->channelTriggerCallback = channelTriggerCallback;
}
void simpleSerialDecoder::attachChannelGateCallback(void(*channelGateCallback)(uint8_t channel, uint8_t number)){
	this->channelGateCallback = channelGateCallback;
}
void simpleSerialDecoder::attachChannelCVCallback(void(*channelCVCallback)(uint8_t channel, uint8_t number)){
	this->channelCVCallback = channelCVCallback;
}
void simpleSerialDecoder::attachChannelInterpolateFromCallback(void(*channelInterpolateFromCallback)(uint8_t channel, uint8_t number)){
	this->channelInterpolateFromCallback = channelInterpolateFromCallback;
}
void simpleSerialDecoder::attachChannelInterpolateToCallback(void(*channelInterpolateToCallback)(uint8_t channel, uint8_t number)){
	this->channelInterpolateToCallback = channelInterpolateToCallback;
}
void simpleSerialDecoder::attachChannelInterpolateCallback(void(*channelInterpolateCallback)(uint8_t channel, uint8_t number)){
	this->channelInterpolateCallback = channelInterpolateCallback;
}
void simpleSerialDecoder::attachChannelModeCallback(void(*channelModeCallback)(uint8_t channel, uint8_t number)){
	this->channelModeCallback = channelModeCallback;
}
void simpleSerialDecoder::attachChannelValueCallback(void(*channelValueCallback)(uint8_t channel, uint8_t value, uint8_t number)){
	this->channelValueCallback = channelValueCallback;
}
