#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define SPI_CH 0
#define ADC_CH0 0
#define ADC_CS 8
#define SPI_SPEED 500000

#include "LightSensor.h"

int InitLightSensor(void) {
    if (wiringPiSetup() == -1) return -1;

    pinMode(ADC_CS, OUTPUT);

    if (wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1) {
        printf("wiringPi SPI 설정 실패\n");
        exit(0);
    }

    return 0;
}

int ReadLightSensor(void) {
    unsigned char buf[3];
    int accValue = 0;
    char adChannel = ADC_CH0;

    buf[0] = 0x06 | ((ADC_CH0 & 0x07) >> 2);
    buf[1] = ((ADC_CH0 & 0x07) << 6);
    buf[2] = 0x00;

    digitalWrite(ADC_CS, 0);

    wiringPiSPIDataRW(SPI_CH, buf, 3);

    buf[1] = 0x0F & buf[1];
    accValue = (buf[1] << 8) | buf[2];

    digitalWrite(ADC_CS, 1);

    return accValue;
}