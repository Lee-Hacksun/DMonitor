#ifndef ACTUATOR_H
#define ACTUATOR_H

#if TEST
#else
#include "Actuator/BuzzerActuator.h"
#include "Actuator/LedActuator.h"

int InitActuator(void);
#endif

void RunActuator(int buzzer, int red, int green, int blue);

#endif