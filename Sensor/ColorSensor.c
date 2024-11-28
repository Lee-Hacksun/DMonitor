#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>

#define COLOR_S0 0
#define COLOR_S1 1
#define COLOR_S2 2
#define COLOR_S3 3
#define COLOR_OUT 4
#define COLOR_LED 5

#include "colorSensor.h"

void Count()
{
    counter++;
}

int InitColorSensor(void)
{
    if (wiringPiSetup() == -1)
        return -1;

    pinMode(COLOR_S0, OUTPUT);
    pinMode(COLOR_S1, OUTPUT);
    pinMode(COLOR_S2, OUTPUT);
    pinMode(COLOR_S3, OUTPUT);
    pinMode(COLOR_OUT, INPUT);
    pinMode(COLOR_LED, OUTPUT);

    digitalWrite(COLOR_S0, 0);
    digitalWrite(COLOR_S1, 1);

    digitalWrite(COLOR_LED, 1);

    wiringPiISR(COLOR_OUT, INT_EDGE_RISING, &Count);
}

int DetectColor(int color)
{
    switch (color)
    {
    case 0:
        digitalWrite(COLOR_S2, LOW);
        digitalWrite(COLOR_S3, LOW);
        break;
    case 1:
        digitalWrite(COLOR_S2, HIGH);
        digitalWrite(COLOR_S3, HIGH);
        break;
    case 2:
        digitalWrite(COLOR_S2, LOW);
        digitalWrite(COLOR_S3, HIGH);
        break;
    }
}

int ReadColorSensor(ColorData* data) {
    if (counter == NULL) return -1;

    DetectColor(0);
    counter = 0;
    delay(100);
    data -> red = counter;

    DetectColor(1);
    counter = 0;
    delay(100);
    data -> green = counter;

    DetectColor(2);
    counter = 0;
    delay(100);
    data -> blue = counter;

    return 0;
}