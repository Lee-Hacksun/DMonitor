#include <wiringPi.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <stdint.h>  

#define MAX_TIME 100  
#define DHT11PIN 25

#include "Dht11Sensor.h"

static int dht11_val[5] = {0, 0, 0, 0, 0};
static float farenheit_temp;

int InitDht11Sensor(void) {
    if(wiringPiSetup() == -1)
        return -1;
    return 0;
}

DHT11_Data ReadDht11Data(void) {
    DHT11_Data data = {-100, -100};

    unsigned char laststate = HIGH;
    unsigned char counter = 0;
    unsigned char j = 0;

    for (int i = 0; i < 5; i++)
        dht11_val[i] = 0;

    pinMode(DHT11PIN, OUTPUT);
    digitalWrite(DHT11PIN, 0);
    delay(18);
    digitalWrite(DHT11PIN, 1);
    delayMicroseconds(40);
    pinMode(DHT11PIN, INPUT);

    for (int i = 0; i < MAX_TIME; i++) {
        counter = 0;
        while (digitalRead(DHT11PIN) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = digitalRead(DHT11PIN);
        if (counter == 255) break;

        if((i >= 4) && (i % 2 == 0)) {
            dht11_val[j/8] <<= 1;
            if (counter > 16) dht11_val[j/8] |= 1;
            j++;
        }
    }

    return data;
}