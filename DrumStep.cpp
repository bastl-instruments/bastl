#include "DrumStep.h"

DrumStep::DrumStep() : Step(false) {
    for (int i = 0; i < 4; i++) {
        _substeps[i] = OFF;
    }
}

DrumStep::DrumStep(bool mute, DrumVelocityType substeps[]) : Step(mute)
{
    for (int i = 0; i < 4; i++) {
        _substeps[i] = substeps[i];
    }
}
