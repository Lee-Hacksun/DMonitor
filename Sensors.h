#ifndef SENSORS_H
#define SENSORS_H

    #if TEST
        // 색상 문자열 생성
        void ColorToString(int red, int green, int blue, char *colorString);

        // 정수 난수 생성
        int getRandomRange(int min, int max);

        // 실수 난수 생성
        float getRandomRangeFloat(float min, float max);
    #else
        #include "Sensor/ColorSensor.c"
        #include "Sensor/Dht11Sensor.c"
        #include "Sensor/FlameSensor.c"
        #include "Sensor/GasSensor.c"
        #include "Sensor/LightSensor.c"

        typedef struct
        {
            DHT11_Data dht11;
            ColorData color;
            int flame;
            int gas;
            int light;
        } SensorData;

        // 색상 문자열 생성
        void ColorToString(SensorData *sensorData, char *colorString);

        // 센서 초기화
        int InitSensors(void);

        // 센서 데이터 읽기
        SensorData ReadSensors(void);
    #endif

    // 센서 실행
    void runSensors(void);
#endif
