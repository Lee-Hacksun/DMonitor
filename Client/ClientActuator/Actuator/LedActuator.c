#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define LED_RED 7 //4
#define LED_GREEN 21 // 5
#define LED_BLUE 22   //6

#include "LedActuator.h"

int InitLED(void) {
    if (wiringPiSetup() == -1) return -1;

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    TurnOffAllLED();

    return 0;
}

void ControlRedLED(int state) {
    digitalWrite(LED_RED, state);
}

void ControlGreenLED(int state) {
    digitalWrite(LED_GREEN, state);
}

void ControlBlueLED(int state) {
    digitalWrite(LED_BLUE, state);
}

void TurnOffAllLED(void) {
    ControlRedLED(0);
    ControlGreenLED(0);
    ControlBlueLED(0);
}