/*
 * StepSynchronizer.cpp
 *
 *  Created on: Sep 12, 2014
 *      Author: bastl
 */

#include "StepSynchronizer.h"

StepSynchronizer::StepSynchronizer() : cycleLength_(256), currentStepNumber_ (0), waiting_(true) {
}
