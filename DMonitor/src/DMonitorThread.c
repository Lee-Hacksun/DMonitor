#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "csv.h"
#include "cjson/cJSON.h"
#include "ClientManager.h"
#include "DebugUtil.h"
#include "DMonitorThread.h"
#include "SensorData.h"

int clientCount = 0;
int clientSockets[MAX_CONNECTION];
pthread_mutex_t mutex;

TotalData* InitTotalData()
{
    TotalData* totalData = malloc(sizeof(TotalData));
    totalData->count = 0;

    return totalData;
}

// TODO : 테스트 필요
void ProcessDataLog(char* clientID, TotalData* totalData)
{
    SensorData avgSensorData;

    for (int i = 0; i < totalData->count; i++)
    {
        avgSensorData.flame += totalData->sensorData[i]->flame;
        avgSensorData.gas += totalData->sensorData[i]->gas;
        avgSensorData.humidity += totalData->sensorData[i]->humidity;
        avgSensorData.light += totalData->sensorData[i]->light;
        avgSensorData.temp += totalData->sensorData[i]->temp;
    }

    avgSensorData.flame /= totalData->count;
    avgSensorData.gas /= totalData->count;
    avgSensorData.humidity /= totalData->count;
    avgSensorData.light /= totalData->count;
    avgSensorData.temp /= totalData->count;

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s%s/%s.log", EXEPath, LOG_PATH, clientID);
    FILE* sensorLog = fopen(path, "r");
    if (sensorLog == NULL)
    {
        printf("센서 데이터 파일을 생성합니다.\n");
        FILE* sensorLog = fopen(path, "a+");
        if (sensorLog == NULL)
        {
            perror("fopen");
            return;
        }
        fprintf(sensorLog, "ClientID,flame,gas,humidity,light,temp\n");
    }

    fprintf(sensorLog, "%s,%s,%s,%s,%s,%s", clientID, avgSensorData.flame, avgSensorData.gas, avgSensorData.humidity, avgSensorData.light, avgSensorData.temp);
    fclose(sensorLog);
}

char* RegistClient(int clientSocket)
{   
    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    readSize = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (readSize == -1)
    {
        fprintf(stderr, "read 에러\n");
        return NULL;
    }
    buffer[readSize] = '\0';

    cJSON* json = cJSON_Parse(buffer);
    char* clientName = cJSON_GetObjectItem(json, "clientID")->valuestring;

    char path[PATH_MAX];
    strcpy(path, EXEPath);
    strcat(path, CLIENT_LIST_PATH);

    FILE* clientList = fopen(path, "a+");
    if (clientList == NULL)
    {
        perror("fopen");
        return NULL;
    }

    int isExist = 0;
    CSV* csv = ParseCSV(clientList);
    for (int i = 0; i < csv->csvSize; i++)
    {
        if (strcmp(csv->data[i][CLIENT_LIST_CSV_CLIENT_ID], clientName) == 0)
        {
            isExist = 1;
            break;
        }
    }
    DestroyCSV(csv);


    if(!isExist)
    {
        fprintf(clientList, "%s,432\n", clientName);
        printf("클라이언트가 새로 등록되었습니다. : %s\n", clientName);
    }

    fclose(clientList);
}

SensorData* ReadSensorData(int clientSocket)
{
    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    readSize = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (readSize == -1)
    {
        fprintf(stderr, "네트워크 에러");
        return NULL;
    }
    else if (readSize == 0)
    {
        printf("클라이언트가 연결을 종료했습니다.\n");
        return NULL;
    }
    buffer[readSize] = '\0';
    printf("클라이언트로부터 받은 센서 데이터: %s\n", buffer);
    
    SensorData* sensorData = mallco(sizeof(SensorData));
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        ASSERT(0, "Failed to parse json");
        return NULL;
    }

    sensorData->flame = cJSON_GetObjectItem(json, "flame")->valueint;
    sensorData->gas = cJSON_GetObjectItem(json, "gas")->valueint;
    sensorData->humidity = cJSON_GetObjectItem(json, "humidity")->valueint;
    sensorData->light = cJSON_GetObjectItem(json, "light")->valueint;
    sensorData->temp = cJSON_GetObjectItem(json, "temp")->valueint;

    return sensorData;
}

void* DMonitorThreadAction(void* arg)
{
    int clientSocket = *(int*)arg;
    char* clientID = RegistClient(clientSocket);
    TotalData* totalData = InitTotalData();

    time_t currentTime;
    struct tm* localTime;
    int currentDay;

    int isLEDSelected = 0;
    Color color = {0, 0, 0};

    time(&currentTime);
    localtime_r(&currentTime, &localTime);
    currentDay = localTime->tm_mday;

    while(1)
    {
        time(&currentTime);
        localtime_r(&currentTime, &localTime);
        if (currentDay != localTime->tm_mday)
        {
            ProcessDataLog(clientID, totalData);
            currentDay = localTime->tm_mday;
        }

        SensorData* sensorData = ReadSensorData(clientSocket);
        if (sensorData == NULL)
        {
            break;
        }
        totalData->sensorData[totalData->count++] = sensorData;

        ReadLock(&g_color_rwlock);
        if (strcmp(g_colorClientID, clientID) == 0)
        {   
            isLEDSelected = 1;
            color.Red = g_color.Red;
            color.Green = g_color.Green;
            color.Blue = g_color.Blue;
        }
        else
        {
            isLEDSelected = 0;
        }
        ReadUnlock(&g_color_rwlock);


        // FILE* log = fopen("test.txt", "a");
        // ReadLock(&g_rwlock);
        // fprintf(log, "%d:%d:%d\n", g_color.Red, g_color.Green, g_color.Blue);
        // ReadUnlock(&g_rwlock);
        // fflush(log);
        // sleep(5);

    }
    free(clientID);
    close(clientSocket);
    exit(1);
}

/*

센서로부터 값 읽기
제어코드 변수 읽기
제어코드 실행
일정 시간에 도달하면 평균 내서 로깅 

*/

void InitDMonitorThread()
{
    pthread_mutex_init(&mutex, NULL);
}

void DMonitorThreadCreate(int clientSocket)
{
    pthread_t threadID = 0;

    int* _clientSocket = malloc(sizeof(int));
    *_clientSocket = clientSocket;

    pthread_create(&threadID, NULL, DMonitorThreadAction, (void*)_clientSocket);
    pthread_detach(threadID);
}
