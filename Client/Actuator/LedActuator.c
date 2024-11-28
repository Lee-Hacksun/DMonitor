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

int ControlLED(int color, LEDSTATE state) {
    if (color != LED_RED && color != LED_GREEN && color != LED_BLUE) return -1;

    digitalWrite(color, state);

    return 0;
}

int SetColor(LEDSTATE red, LEDSTATE green, LEDSTATE blue) {
    ControlLED(LED_RED, red);
    ControlLED(LED_GREEN, green);
    ControlLED(LED_BLUE, blue);

    return 0;
}

void TurnOffAllLED(void) {
    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);
}