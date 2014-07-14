#ifndef SRAMHWMOCK_H
#define SRAMHWMOCK_H

#include "../../hw/IHWLayer.h"

class SRAMHWMock : public IHWLayer
{
public:
    SRAMHWMock();
    virtual bool buttonDown(unsigned char index);
    virtual void setDiod(unsigned char index);
    virtual unsigned char getKnobValue(unsigned char index);
    virtual unsigned char readSRAM(long address);
    virtual void readSRAMMore(long address, unsigned char * data, unsigned char size);
    virtual void writeSRAM(long address, unsigned char data);
    virtual void writeSRAMMore(long address, unsigned char * data, unsigned char size);
private:
    unsigned char data_[256000];
};

#endif // SRAMHWMOCK_H
