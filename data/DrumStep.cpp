#include "DrumStep.h"

DrumStep::DrumStep() : Step(true, false) {
    for (int i = 0; i < 4; i++) {
        _substeps[i] = OFF;
    }
}

DrumStep::DrumStep(bool active, bool mute, DrumVelocityType substeps[]) : Step(active, mute)
{
    for (int i = 0; i < 4; i++) {
        _substeps[i] = substeps[i];
    }
}
