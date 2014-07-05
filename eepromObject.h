/*
 * eepromObject.h
 *
 *  Created on: 03.07.2014
 *      Author: user
 */

#ifndef EEPROMOBJECT_H_
#define EEPROMOBJECT_H_

#include <new.h>
#include <EEPROM.h>
#include <debugSwitch.h>


/*
Base class for the EEPROMObject class
It's only purpose is to track the position of the last bit in EEPROM that is managed by EEPROMObject
@note: There is no constructor implemented to prevent the creation of instances
 */

class EEPROMBaseObject {

protected:
	static uint16_t totalSize;
};

uint16_t EEPROMBaseObject::totalSize = 0;



/*
Class that allows to pack subsets of bits of POD data types into EEPROM
In order to use every bit of EEPROM memory, variables need to concatenated. Further more, if a variable is limited to a maximum value smaller that the maximum value of the data type,
only the bits can be stored.
Every instance contains a public member VALUE that serves as a buffer for that variable in RAM. With store() and load(), this buffer is written to EEPROM.
@note A boundary check is not performed. Values that are greater than the range provided by the number of stored bits will be clipped!
 */


template <typename baseType>
class EEPROMObject : public EEPROMBaseObject {

public:
	// default constructor. should not be called!
	//EEPROMObject();

	// create object. eeprom is not accessed
	EEPROMObject(uint8_t numBits);

	// write the buffer to the eeprom
	void store();

	//shortcut to set RAM buffer and write to EEPROM
	void store(baseType value);

	// reads the value from eeprom the buffer and returns it
	baseType load();

	// the buffer of the actual value. can be modified independent from eeprom memory
	// allowing this direct access can save one external variable
	baseType value;

	// buffer for the default value. read and write directly
	baseType defaultValue;

private:

	uint8_t storedBits;     // the number of bits that are actually used in EEPROM. this of course limits the maximum size of the storable value
	uint16_t offset;        // index of the first bit in eeprom that belongs to this object

};



template <typename baseType>
EEPROMObject<baseType>::EEPROMObject(uint8_t numBits) {
	storedBits = numBits;
	offset = totalSize;      // start at next unused bit
	totalSize += storedBits; // increase the total size of all concatenated eeprom objects
	defaultValue = 0;
}


template <typename baseType>
void EEPROMObject<baseType>::store(baseType writeValue) {
	value = writeValue;
	store();
}


template <typename baseType>
void EEPROMObject<baseType>::store() {
	uint16_t posEEPROM = offset; // position of bit in eeprom

	DBGS Serial.print("Saving "); Serial.print(value); Serial.print(" at "); Serial.println(posEEPROM); DBGE

	// get the first affected byte
	uint8_t eepromByte = EEPROM.read(posEEPROM/8);

	DBGS Serial.print("Read Byte: "); Serial.println(eepromByte,BIN); DBGE

	//go through the bits that should be written to eeprom
	for (uint8_t numBit = storedBits; numBit>0; numBit--) {  //form X to 1 MSB-->LSB

		DBGS Serial.print("Storing bit "); Serial.println(numBit,DEC); DBGE

		bool bitValueToWrite = bitRead(value,numBit-1);
		uint8_t posInTargetByte = (7 - posEEPROM % 8);

		DBGS Serial.println(bitValueToWrite); DBGE
		DBGS Serial.println(posInTargetByte); DBGE

		bitWrite(eepromByte,posInTargetByte,bitValueToWrite);    //modify bit in EEPROM byte

		posEEPROM++; // move reading head

		if (posEEPROM % 8 == 0) {  // if border to new byte is crossed
			EEPROM.write((posEEPROM-1)/8,eepromByte);  // save modified byte to eeprom
			eepromByte = EEPROM.read(posEEPROM/8);         // ..and get the next byte
			DBGS Serial.print("Reading next byte : "); Serial.println(eepromByte,BIN); DBGE
		}
	}

	EEPROM.write(posEEPROM/8, eepromByte); //save modifications on eeprom byte

}

template <typename baseType>
baseType EEPROMObject<baseType>::load() {

	uint16_t readPos = offset;

	DBGS Serial.print("Reading "); Serial.print(storedBits); Serial.print(" bits from "); Serial.println(offset); DBGE

	//get the first affected byte
	uint8_t eepromByte = EEPROM.read(readPos/8);

	DBGS Serial.print("Read Byte: "); Serial.println(eepromByte,BIN); DBGE

	// clear value
	value = 0;

	// go through all the stored bits
	for (uint8_t numBit = storedBits; numBit>0; numBit--) {

		DBGS Serial.print("Reading bit "); Serial.println(numBit,DEC); DBGE

		bool bitValueToRead = bitRead(eepromByte,7-(readPos%8)); //reading the bit that readPos is pointing at
		bitWrite(value,numBit-1,bitValueToRead);

		readPos++; // move bit pointer one step ahead

		if (readPos % 8 == 0) { //if border to new byte has been crossed
			eepromByte = EEPROM.read(readPos/8);
			DBGS Serial.print("Reading next byte : "); Serial.println(eepromByte,BIN); DBGE
		}
	}

	return value;
}




#endif /* EEPROMOBJECT_H_ */
