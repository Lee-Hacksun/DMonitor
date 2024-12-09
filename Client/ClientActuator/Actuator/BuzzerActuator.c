#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define BUZZER 15

#include "BuzzerActuator.h"

int InitBuzzer(void) {
    if (wiringPiSetup() == -1) return -1;

    pinMode(BUZZER, OUTPUT);

    digitalWrite(BUZZER, 0);

    return 0;
}

void ControlBuzzer(int state) {
    digitalWrite(BUZZER, state);
}