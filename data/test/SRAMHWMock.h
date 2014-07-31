#ifndef SRAMHWMOCK_H
#define SRAMHWMOCK_H

#include "../../hw/IHWLayer.h"

class SRAMHWMock : public IHWLayer
{
public:
    SRAMHWMock();
    virtual IHWLayer::ButtonState getButtonState(unsigned char index);
    virtual void setLED(unsigned char index, IHWLayer::LedState state);
    virtual unsigned char getKnobValue(unsigned char index);
    virtual unsigned char readSRAM(long address);
    virtual void readSRAM(long address, unsigned char * data, unsigned int size);
    virtual void writeSRAM(long address, unsigned char data);
    virtual void writeSRAM(long address, unsigned char * data, unsigned int size);
    virtual unsigned int getElapsedBastlCycles() {return 0;};
    virtual unsigned int getBastlCyclesPerSecond() {return 0;};

private:
    unsigned char data_[256000];
};

#endif // SRAMHWMOCK_H
