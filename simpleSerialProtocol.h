/*
 * simpleSerialProtocol.h
 *
 *  Created on: Jan 23, 2015
 *      Author: dasvaclav
 */

#ifndef SIMPLESERIALPROTOCOL_H_
#define SIMPLESERIALPROTOCOL_H_
#include <Arduino.h>
#include <inttypes.h>

class simpleSerialProtocol {

	public:
		void init(uint16_t baudrate);
		void transferMessage(uint8_t _type,uint8_t _value);
	//	void decodeMessage();
		bool recieveMessage();
		uint8_t getMessageType();
		uint8_t getMessageValue();
		//handshake if necessary

	private:
		uint8_t type;
		uint8_t value;
		uint8_t count;

};



#endif /* SIMPLESERIALPROTOCOL_H_ */
