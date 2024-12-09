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
    int clamp(int value, int min, int max) {
        value = value / 3000 * 250;
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    int InitSensors(void)
    {
        if (InitColorSensor() == -1)
        {
            printf("Color sensor initialization failed\n");
            return -1;
        }

        if (InitDht11Sensor() == -1)
        {
            printf("DHT11 sensor initialization failed\n");
            return -1;
        }

        if (InitFlameSensor() == -1)
        {
            printf("Flame sensor initialization failed\n");
            return -1;
        }

        if (InitGasSensor() == -1)
        {
            printf("Gas sensor initialization failed\n");
            return -1;
        }

        if (InitLightSensor() == -1)
        {
            printf("Light sensor initialization failed\n");
            return -1;
        }

        return 0;
    }

    SensorData ReadSensors(void)
    {
        SensorData sensorData;
        ColorData colorData;

        ReadColorSensor(&colorData);
        sensorData.color = colorData;

        sensorData.dht11 = ReadDht11Sensor();

        sensorData.flame = ReadFlameSensor() >= 30 ? 1 : 0;

        sensorData.gas = ReadGasSensor();

        sensorData.light = ReadLightSensor();

        return sensorData;
    }
#endif

SensorData runSensors(void)
{
    SensorData sensorData;
    
    #if TEST
        srand(time(NULL));
        
        float temp = getRandomRangeFloat(-10, 40);
        float humidity = getRandomRangeFloat(0, 95);
        int colorRed = getRandomRange(0, 255);
        int colorGreen = getRandomRange(0, 255);
        int colorBlue = getRandomRange(0, 255);
        int light = getRandomRange(0, 10000);
        int flame = getRandomRange(0, 1);
        int gas = getRandomRange(0, 100000);

        sensorData.dht11.temperature = temp;
        sensorData.dht11.humidity = humidity;
        sensorData.color.red = colorRed;
        sensorData.color.green = colorGreen;
        sensorData.color.blue = colorBlue;
        sensorData.light = light;
        sensorData.flame = flame;
        sensorData.gas = gas;

        printf("Current temperature: %.1f°C\n", sensorData.dht11.temperature);
        printf("Current humidity: %.1f%%\n", sensorData.dht11.humidity);
        printf("Current color: %d;%d;%d\n", sensorData.color.red, sensorData.color.green, sensorData.color.blue);
        printf("Current light: %d\n", sensorData.light);
        printf("Current flame detection: %s\n", sensorData.flame ? "true" : "false");
        printf("Current gas concentration: %d\n", sensorData.gas);

        return sensorData;
    #else
        if (InitSensors() == -1)
        {
            printf("Sensor initialization failed\n");
            return sensorData;
        }

        while (1)
        {
            SensorData sensorData = ReadSensors();

            sensorData.color.red = clamp(sensorData.color.red, 0, 255);
            sensorData.color.green = clamp(sensorData.color.green, 0, 255);
            sensorData.color.blue = clamp(sensorData.color.blue, 0, 255);

            printf("Current temperature: %.1f°C\n", sensorData.dht11.temperature);
            printf("Current humidity: %.1f%%\n", sensorData.dht11.humidity);
            printf("Current color: %d;%d;%d\n", sensorData.color.red, sensorData.color.green, sensorData.color.blue);
            printf("Current light: %d\n", sensorData.light);
            printf("Current flame detection: %s\n", sensorData.flame ? "true" : "false");
            printf("Current gas concentration: %d\n", sensorData.gas);

            return sensorData;
        }
    #endif
}