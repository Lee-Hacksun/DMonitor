#include <stdio.h>
#include <string.h>

#include "Sensor/ColorSensor.c"
#include "Sensor/Dht11Sensor.c"
#include "Sensor/FlameSensor.c"
#include "Sensor/GasSensor.c"
#include "Sensor/LightSensor.c"

typedef struct _SensorData
{
    DHT11_Data dht11;
    ColorData color;
    int flame;
    int gas;
    int light;
} SensorData;

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

void ColorToString(ColorData colorData, char *colorString)
{
    sprintf(colorString, "%d;%d;%d", colorData.red, colorData.green, colorData.blue);
}

int main(void)
{
    const char* CLIENT_ID = "SENSOR01";  // 클라이언트 ID 설정
    
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
}