#ifndef SENSORS_H
#define SENSORS_H

    #if TEST
        // 정수 난수 생성
        int getRandomRange(int min, int max);

        // 실수 난수 생성
        float getRandomRangeFloat(float min, float max);

        typedef struct {
            float temperature;
            float humidity;
        } Dht11Data;

        typedef struct {
            int red;
            int green;
            int blue;
        } ColorData;

        typedef struct {
            char clientId[20];
            Dht11Data dht11;
            ColorData color;
            int light;
            int flame;
            int gas;
        } SensorData;


    #else
        #include "ColorSensor.h"
        #include "Dht11Sensor.h"
        #include "FlameSensor.h"
        #include "GasSensor.h"
        #include "LightSensor.h"

        typedef struct _SensorData
        {
            char clientId[20];
            DHT11_Data dht11;
            ColorData color;
            int flame;
            int gas;
            int light;
        } SensorData;

        // 센서 초기화
        int InitSensors(void);

        // 센서 데이터 읽기
        SensorData ReadSensors(void);
    #endif

    // 센서 실행
    SensorData runSensors(void);
#endif
