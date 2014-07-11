#ifndef IHWLAYER_H
#define IHWLAYER_H

#define BUTTON_DRUM_1 16
#define BUTTON_DRUM_2 17
#define BUTTON_DRUM_3 18

class IHWLayer {
public:
    virtual bool buttonDown(unsigned char index) = 0;
    virtual void setDiod(unsigned char index) = 0;
    virtual unsigned char getKnobValue(unsigned char index) = 0;
    virtual unsigned char readSRAM(long address) = 0;
    virtual void readSRAMMore(long address, unsigned char * data, unsigned char size) = 0;
    virtual void writeSRAM(long address, unsigned char data) = 0;
    virtual void writeSRAMMore(long address, unsigned char * data, unsigned char size) = 0;

};

#endif // IHWLAYER_H
