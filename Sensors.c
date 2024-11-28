#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Sensors.h"
#include "DebugUtil.h"
#include "SensorLog.h"

#if TEST
    int getRandomRange(int min, int max) {
        return min + (rand() % (max - min + 1));
    }

    float getRandomRangeFloat(float min, float max) {
        float scale = rand() / (float) RAND_MAX;
        return min + scale * (max - min);
    }

    void ColorToString(int red, int green, int blue, char *colorString)
    {
        sprintf(colorString, "%d;%d;%d", red, green, blue);
    }
#else
    void ColorToString(SensorData *sensorData, char *colorString)
    {
        sprintf(colorString, "%d;%d;%d", sensorData->color.red, sensorData->color.green, sensorData->color.blue);
    }

    int InitSensors(void)
    {
        if (InitColorSensor() == -1)
        {
            printf("컬러 센서 초기화 실패\n") return -1;
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

        // 컬러 센서 읽기
        ReadColorSensor(&colorData);
        data.color = colorData;

        // DHT11 센서 읽기
        data.dht11 = ReadDht11Sensor();

        // 화재 센서 읽기
        sensorData.flame = ReadFlameSensor();

        // 가스 센서 읽기
        sensorData.gas = ReadGasSensor();

        // 조도 센서 읽기
        sensorData.light = ReadLightSensor();

        return sensorData;
    }
#endif

void runSensors(void)
{
    const char* CLIENT_ID = "SENSOR01";  // 클라이언트 ID 설정
    
    #if TEST
        // TODO : 테스트를 위한 코드
        srand(time(NULL));

        for (int i = 0; i < 100; i++) {
            float temp = getRandomRangeFloat(-10, 40);
            float humidity = getRandomRangeFloat(0, 95);
            int colorRed = getRandomRange(0, 255);
            int colorGreen = getRandomRange(0, 255);
            int colorBlue = getRandomRange(0, 255);
            int light = getRandomRange(0, 10000);
            int flame = getRandomRange(0, 1000);
            int gas = getRandomRange(0, 100000);

            char colorStr[16];
            ColorToString(colorRed, colorGreen, colorBlue, colorStr);

            // 로그파일에 기록
            WriteLog(CLIENT_ID,
                    temp,
                    humidity,
                    colorStr,
                    light,
                    flame,
                    gas);
        }
    #else
        if (InitSensors() == -1)
        {
            printf("센서 초기화 실패\n");
            return -1;
        }

        while (1)
        {
            SensorData sensorData = ReadSensors();
            char colorStr[16];
            ColorToString(sensorData.color, colorStr);

            printf("온도: %.1f°C\n", sensorData.dht11.temperature);
            printf("습도: %.1f%\n", sensorData.dht11.humidity);
            printf("색상: %s\n", colorStr);
            printf("조도: %d\n", sensorData.light);
            printf("화재: %d\n", sensorData.flame);
            printf("가스: %d\n", sensorData.gas);

            // 로그파일에 기록
            WriteLog(CLIENT_ID,
                    sensorData.dht11.temperature,
                    sensorData.dht11.humidity,
                    colorStr,
                    sensorData.light,
                    sensorData.flame,
                    sensorData.gas);

            sleep(1);
        }
    #endif
}