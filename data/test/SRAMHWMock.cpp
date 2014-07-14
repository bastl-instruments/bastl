#include "SRAMHWMock.h"

SRAMHWMock::SRAMHWMock()
{
    for (long i = 0; i < 256000; i++) {
        data_[i] = 255;
    }
}

bool SRAMHWMock::buttonDown(unsigned char index)
{
    return false;
}

void SRAMHWMock::setDiod(unsigned char index)
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

void SRAMHWMock::readSRAMMore(long address, unsigned char *data, unsigned char size)
{
    for (unsigned char i = 0; i < size; i++) {
        data[i] = data_[i + address];
    }
}

void SRAMHWMock::writeSRAM(long address, unsigned char data)
{
    data_[address] = data;
}

void SRAMHWMock::writeSRAMMore(long address, unsigned char *data, unsigned char size)
{
    for (unsigned char i = 0; i < size; i++) {
        data_[i + address] = data[i];
    }
}
