#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define SPI_CH 0
#define ADC_CS 8
#define ADC_CH7 7
#define SPI_SPEED 500000

#include "FlameSensor.h"

int InitFlameSensor(void) {
    if (wiringPiSetup() == -1) return -1;

    pinMode(ADC_CS, OUTPUT);

    if (wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1) {
        printf("wiringPi SPI 설정 실패\n");
        exit(0);
    }

    return 0;
}

int ReadFlameSensor(void) {
    unsigned char buf[3];
    int adcValue = 0;
    char adChannel = ADC_CH7;

    buf[0] = 0x06 | ((adChannel & 0x07) >> 2);
    buf[1] = ((adChannel & 0x07) << 6);
    buf[2] = 0x00;

    digitalWrite(ADC_CS, 0);
    wiringPiSPIDataRW(SPI_CH, buf, 3);

    buf[1] = 0x0F & buf[1];
    adcValue = (buf[1] << 8) | buf[2];

    digitalWrite(ADC_CS, 1);

    return adcValue;
}