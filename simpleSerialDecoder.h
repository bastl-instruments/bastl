/*
 * simpleSerialDecoder.h
 *
 *  Created on: Jan 23, 2015
 *      Author: dasvaclav
 */

#ifndef SIMPLESERIALDECODER_H_
#define SIMPLESERIALDECODER_H_

#include <Arduino.h>
#include <inttypes.h>
class simpleSerialDecoder {

	public:
		void init(uint16_t baudrate);
		void update();
		void sendClock(uint8_t _number);
		void sendStep(uint8_t _number);
		void sendGate(uint8_t _number);
		void sendTrigger(uint8_t _number);
		void sendStart(uint8_t _number);
		void sendStop(uint8_t _number);
		void sendRestart(uint8_t _number);

		void sendChannelTrigger(uint8_t _channel, uint8_t _number);
		void sendChannelGate(uint8_t _channel, uint8_t _number);
		void sendChannelCV(uint8_t _channel, uint8_t _number);
		void sendChannelInterpolateFrom(uint8_t _channel, uint8_t _number);
		void sendChannelInterpolateTo(uint8_t _channel, uint8_t _number);
		void sendChannelInterpolate(uint8_t _channel, uint8_t _number);
		void sendChannelMode(uint8_t _channel, uint8_t _number);

		void sendChannelValue(uint8_t _channel, uint8_t _value, uint8_t _number);

		void attachClockCallback(void(*clockInCallback)(uint8_t number));
		void attachStepCallback(void(*stepCallback)(uint8_t number));
		void attachGateCallback(void(*gateCallback)(uint8_t number));
		void attachTriggerCallback(void(*triggerCallback)(uint8_t number));
		void attachStartCallback(void(*startCallback)(uint8_t number));
		void attachStopCallback(void(*stopCallback)(uint8_t number));
		void attachRestartCallback(void(*restartCallback)(uint8_t number));
		void attachChannelTriggerCallback(void(*channelTriggerCallback)(uint8_t channel, uint8_t number));
		void attachChannelGateCallback(void(*channelGateCallback)(uint8_t channel, uint8_t number));
		void attachChannelCVCallback(void(*channelCVCallback)(uint8_t channel, uint8_t number));
		void attachChannelInterpolateFromCallback(void(*channelInterpolateFromCallback)(uint8_t channel, uint8_t number));
		void attachChannelInterpolateToCallback(void(*channelInterpolateToCallback)(uint8_t channel, uint8_t number));
		void attachChannelInterpolateCallback(void(*channelInterpolateCallback)(uint8_t channel, uint8_t number));
		void attachChannelModeCallback(void(*channelModeCallback)(uint8_t channel, uint8_t number));
		void attachChannelValueCallback(void(*channelValueCallback)(uint8_t channel, uint8_t value, uint8_t number));

	private:
		uint8_t _type;
		uint8_t _value;
		void (*clockInCallback)(uint8_t number);
		void (*stepCallback)(uint8_t number);
		void (*gateCallback)(uint8_t number);
		void (*triggerCallback)(uint8_t number);
		void (*startCallback)(uint8_t number);
		void (*stopCallback)(uint8_t number);
		void (*restartCallback)(uint8_t number);
		void (*channelTriggerCallback)(uint8_t channel, uint8_t number);
		void (*channelGateCallback)(uint8_t channel, uint8_t number);
		void (*channelCVCallback)(uint8_t channel, uint8_t number);
		void (*channelInterpolateFromCallback)(uint8_t channel, uint8_t number);
		void (*channelInterpolateToCallback)(uint8_t channel, uint8_t number);
		void (*channelInterpolateCallback)(uint8_t channel, uint8_t number);
		void (*channelModeCallback)(uint8_t channel, uint8_t number);
		void (*channelValueCallback)(uint8_t channel, uint8_t value, uint8_t number);


};




#endif /* SIMPLESERIALDECODER_H_ */
