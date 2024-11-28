#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

typedef struct {
    float temperature;
    float humidity;
} DHT11_Data;

// dht11 센서 초기화 함수
int InitDht11Sensor(void);

// dht11 센서 값 읽는 함수
DHT11_Data ReadDht11Data(void);

#endif