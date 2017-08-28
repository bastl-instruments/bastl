#ifndef DRUMSTEP_H
#define DRUMSTEP_H

#include "Step.h"

class DrumStep : public Step
{
public:
    enum DrumVelocityType {UP = 0, DOWN, NORMAL, OFF};
    DrumStep();
    /**
     * @brief DrumStep
     * @param mute
     * @param substeps
     */
    DrumStep(bool mute, DrumVelocityType substeps[]);

    DrumVelocityType getSubStep(unsigned char index);
    void setSubStep(unsigned char index, DrumVelocityType type);
private:
    DrumVelocityType _substeps[4];
};

inline DrumStep::DrumVelocityType DrumStep::getSubStep(unsigned char index) {
    // Be carrefull here dont ask for nonexisting substeps otherwise you get strange behaviour.
    // The if test has been skipped here because it has no sence in this application since we cannot
    // raise exception and returning default value is not correct either. If statement would at the same time
    // steel proccessor time that we could use for other important stuff
    return _substeps[index];
}

inline void DrumStep::setSubStep(unsigned char index,  DrumVelocityType type) {
    // Be carrefulk here dont set nonexisting substeps otherwise you get strange behaviour.
    // The if test has been skipped here because it has no sence in this application since we cannot
    // raise exception and returning default value is not correct either. If statement would at the same time
    // steel proccessor time that we could use for other important stuff
    _substeps[index] = type;
}

#endif // DRUMSTEP_H
