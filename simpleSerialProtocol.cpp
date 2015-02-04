/*
 * simpleSerialProtocol.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: dasvaclav
 */

#include <simpleSerialProtocol.h>

#include <Arduino.h>

/*
simpleSerialProtocol::simpleSerialProtocol(){

}
*/
void simpleSerialProtocol::init(uint16_t baudrate){
	Serial.begin(baudrate);
}
void simpleSerialProtocol::transferMessage(uint8_t _type,uint8_t _value){
	Serial.write(_type);
	Serial.write(_value);
}
#define PAIR_TYPE 9
#define PAIR_VALUE 9

bool simpleSerialProtocol::recieveMessage(){
	if(Serial.available()){
		if(count==0){
			type=Serial.read(), count=1;
			if(type==PAIR_TYPE && value==PAIR_VALUE) sync(); //sync routine
		}
		if(count==1) {
			if(Serial.available()){
				value=Serial.read(), count=0;
				if(type==PAIR_TYPE && value==PAIR_VALUE) sync(); //sync routine
				return true;
			}
			else{
				return false;
			}
		}
	}
	else{
		return false;
	}
}
uint8_t simpleSerialProtocol::getMessageType(){
	return type;
}
uint8_t simpleSerialProtocol::getMessageValue(){
	return value;
}
void simpleSerialProtocol::pairMessage(){
	transferMessage(PAIR_TYPE,PAIR_VALUE);
}

void simpleSerialProtocol::sync(){
	count=0;
	type=0;
	value=0;
}
