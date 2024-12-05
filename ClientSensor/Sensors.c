#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Sensors.h"
#include "DebugUtil.h"

#if TEST
    int getRandomRange(int min, int max) {
        return min + (rand() % (max - min + 1));
    }

    float getRandomRangeFloat(float min, float max) {
        float scale = rand() / (float) RAND_MAX;
        return min + scale * (max - min);
    }
#else
    int InitSensors(void)
    {
        if (InitColorSensor() == -1)
        {
            printf("컬러 센서 초기화 실패\n");
            return -1;
        }

        if (InitDht11Sensor() == -1)
        {
            printf("DHT11 센서 초기화 실패\n");
            return -1;
        }

        if (InitFlameSensor() == -1)
        {
            printf("화재 센서 초기화 실패\n");
            return -1;
        }

        if (InitGasSensor() == -1)
        {
            printf("가스 센서 초기화 실패\n");
            return -1;
        }

        if (InitLightSensor() == -1)
        {
            printf("조도 센서 초기화 실패\n");
            return -1;
        }

        return 0;
    }

    SensorData ReadSensors(void)
    {
        SensorData sensorData;
        ColorData colorData;

        // 클라이언트 ID 설정
        strncpy(sensorData.clientId, "SENSOR01", sizeof(sensorData.clientId) - 1);
        sensorData.clientId[sizeof(sensorData.clientId) - 1] = '\0';

        // 컬러 센서 읽기
        ReadColorSensor(&colorData);
        sensorData.color = colorData;

        // DHT11 센서 읽기
        sensorData.dht11 = ReadDht11Sensor();

        // 화재 센서 읽기
        sensorData.flame = ReadFlameSensor() >= 40 ? 1 : 0;

        // 가스 센서 읽기
        sensorData.gas = ReadGasSensor();

        // 조도 센서 읽기
        sensorData.light = ReadLightSensor();

        return sensorData;
    }
#endif

SensorData runSensors(void)
{
    SensorData sensorData;
    
    #if TEST
        // TODO : 테스트를 위한 코드
        srand(time(NULL));
        
        float temp = getRandomRangeFloat(-10, 40);
        float humidity = getRandomRangeFloat(0, 95);
        int colorRed = getRandomRange(0, 255);
        int colorGreen = getRandomRange(0, 255);
        int colorBlue = getRandomRange(0, 255);
        int light = getRandomRange(0, 10000);
        int flame = getRandomRange(0, 1);
        int gas = getRandomRange(0, 100000);

        // 클라이언트 ID 설정
        strncpy(sensorData.clientId, "SENSOR01", sizeof(sensorData.clientId) - 1);
        sensorData.clientId[sizeof(sensorData.clientId) - 1] = '\0';

        sensorData.dht11.temperature = temp;
        sensorData.dht11.humidity = humidity;
        sensorData.color.red = colorRed;
        sensorData.color.green = colorGreen;
        sensorData.color.blue = colorBlue;
        sensorData.light = light;
        sensorData.flame = flame;
        sensorData.gas = gas;

        printf("클라이언트 ID: %s\n", sensorData.clientId);
        printf("현재 온도: %.1f°C\n", sensorData.dht11.temperature);
        printf("현재 습도: %.1f%%\n", sensorData.dht11.humidity);
        printf("현재 색상: %d;%d;%d\n", sensorData.color.red, sensorData.color.green, sensorData.color.blue);
        printf("현재 조도: %d\n", sensorData.light);
        printf("현재 화재 감지: %s\n", sensorData.flame ? "true" : "false");
        printf("현재 가스 농도: %d\n", sensorData.gas);

        return sensorData;
    #else
        if (InitSensors() == -1)
        {
            printf("센서 초기화 실패\n");
            return sensorData;
        }

        while (1)
        {
            SensorData sensorData = ReadSensors();

            printf("클라이언트 ID: %s\n", sensorData.clientId);
            printf("현재 온도: %.1f°C\n", sensorData.dht11.temperature);
            printf("현재 습도: %.1f%%\n", sensorData.dht11.humidity);
            printf("현재 색상: %d;%d;%d\n", sensorData.color.red, sensorData.color.green, sensorData.color.blue);
            printf("현재 조도: %d\n", sensorData.light);
            printf("현재 화재 감지: %s\n", sensorData.flame ? "true" : "false");
            printf("현재 가스 농도: %d\n", sensorData.gas);

            return sensorData;
        }
    #endif
}