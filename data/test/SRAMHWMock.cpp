#include "SRAMHWMock.h"

SRAMHWMock::SRAMHWMock()
{
    for (long i = 0; i < 256000; i++) {
        data_[i] = 255;
    }
}

IHWLayer::ButtonState SRAMHWMock::getButtonState(unsigned char index)
{
    return IHWLayer::UP;
}

void SRAMHWMock::setLED(unsigned char index, IHWLayer::LedState state)
{

}

unsigned char SRAMHWMock::getKnobValue(unsigned char index)
{
    return 0;
}

unsigned char SRAMHWMock::readSRAM(long address)
{
    return data_[address];
}

void SRAMHWMock::readSRAM(long address, unsigned char *data, unsigned int size)
{
    for (unsigned char i = 0; i < size; i++) {
        data[i] = data_[i + address];
    }
}

void SRAMHWMock::writeSRAM(long address, unsigned char data)
{
    data_[address] = data;
}

void SRAMHWMock::writeSRAM(long address, unsigned char *data, unsigned int size)
{
    for (unsigned char i = 0; i < size; i++) {
        data_[i + address] = data[i];
    }
}
