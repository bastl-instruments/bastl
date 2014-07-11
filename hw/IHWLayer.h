#ifndef IHWLAYER_H
#define IHWLAYER_H

class IHWLayer {
public:
    virtual bool buttonDown(unsigned char index) = 0;
    virtual void setDiod(unsigned char diod) = 0;
    virtual unsigned char getKnobValue(unsigned char index) = 0;
};

#endif // IHWLAYER_H
