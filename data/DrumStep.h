#ifndef DRUMSTEP_H
#define DRUMSTEP_H

#include "Step.h"

class DrumStep : public Step
{
public:
    enum DrumVelocityType {UP, DOWN, NORMAL, OFF};
    DrumStep();
    /**
     * @brief DrumStep
     * @param active
     * @param mute
     * @param substeps
     */
    DrumStep(bool active, bool mute, DrumVelocityType substeps[]);

    DrumVelocityType getSubStep(unsigned char index);
private:
    DrumVelocityType _substeps[4];
};

inline DrumStep::DrumVelocityType DrumStep::getSubStep(unsigned char index) {
    // Be carrefulk here dont ask for nonexisting substeps otherwise you get strange behaviour.
    // The if test has been skipped here because it has no sence in this application since we cannot
    // raise exception and returning default value is not correct either. If statement would at the same time
    // steel proccessor time that we could use for other important stuff
    return _substeps[index];
}

#endif // DRUMSTEP_H
