#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>

#define SPI_CH 0
#define ADC_CS 8
#define ADC_CH6 6
#define SPI_SPEED 500000

#include "GasSensor.h"

int InitGasSensor(void)
{
    if(wiringPiSetup() == -1)
        return -1;
    
    pinMode(ADC_CS, OUTPUT);

    if(wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1) {
        printf("SPI 설정 실패\n");
        return -1;
    }

    return 0;
}

int ReadGasSensor(char channel) {
    unsigned char buf[3];
    int adcValue = 0;

    buf[0] = 0x06 | (channel >> 6 & 0x03);
    buf[1] = (channel << 2) & 0x78;
    buf[2] = 0x00;

    digitalWrite(ADC_CS, 0);

    wiringPiSPIDataRW(SPI_CH, buf, 3);

    buf[1] = 0x1F & buf[1];
    adcValue = (buf[1] << 8) | buf[2];

    digitalWrite(ADC_CS, 1);

    return adcValue;
}