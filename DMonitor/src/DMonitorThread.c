#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "csv.h"
#include "cjson/cJSON.h"
#include "ClientManager.h"
#include "DebugUtil.h"
#include "DMonitorThread.h"
#include "SensorData.h"

TotalData* InitTotalData()
{
    TotalData* totalData = malloc(sizeof(TotalData));
    totalData->count = 0;

    return totalData;
}

int GetColorDistance(Color tarGetColor, Color nowColor)
{
    double distance = sqrt(pow(tarGetColor.Red - nowColor.Red, 2) + pow(tarGetColor.Green - nowColor.Green, 2) + pow(tarGetColor.Blue - nowColor.Blue, 2));
    double maxDistance = 255 * sqrt(3);
    double percentage = 100.0f - (distance / maxDistance) * 100.0f;
    return (int)percentage;
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

        avgSensorData.color.Red += totalData->sensorData[i]->color.Red;
        avgSensorData.color.Green += totalData->sensorData[i]->color.Green;
        avgSensorData.color.Blue += totalData->sensorData[i]->color.Blue;
    }

    avgSensorData.flame /= totalData->count;
    avgSensorData.gas /= totalData->count;
    avgSensorData.humidity /= totalData->count;
    avgSensorData.light /= totalData->count;
    avgSensorData.temp /= totalData->count;

    avgSensorData.color.Red /= totalData->count;
    avgSensorData.color.Green /= totalData->count;
    avgSensorData.color.Blue /= totalData->count;

    char path[PATH_MAX];
    int pathSize =  strlen(EXEPath) + strlen(LOG_DIR_PATH) + strlen(clientID);
    int writtenSize = snprintf(path, sizeof(path), "%s%s/%s.log", EXEPath, LOG_DIR_PATH, clientID);
    if(writtenSize < 0 || writtenSize >= pathSize)
    {
        fprintf(stderr, "출력이 잘렸습니다.");
        return;
    }

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
        fprintf(sensorLog, "ClientID,flame,gas,humidity,light,temp,Red,Green,Blue,ColorDistance\n");
    }

    fprintf(sensorLog, "%s,%d,%d,%d,%d,%d,%d,%d,%d",
    clientID, 
    avgSensorData.flame, 
    avgSensorData.gas, 
    avgSensorData.humidity, 
    avgSensorData.light, 
    avgSensorData.temp, 
    avgSensorData.color.Red, 
    avgSensorData.color.Green, 
    avgSensorData.color.Blue,
    GetColorDistance(GetRegisteredColor(clientID), avgSensorData.color));

    fclose(sensorLog);
}

// TODO : 테스트 필요
void SetupControlCode(unsigned char code)
{
    int isChaged = 0;
    ReadLock(&g_control_code_rwlock);
    if (code & IS_SUPER_DO)
    {
        isChaged = 1;
    }
    else if ((code & EMERGENCY) & (~(g_control_code & IS_SUPER_DO)))
    {
        isChaged = 1;
    }
    else if ((code & IS_CHANGED) & ((g_control_code & (IS_SUPER_DO | EMERGENCY)) == 0))
    {
        isChaged = 1;
    }
    ReadUnlock(&g_control_code_rwlock);

    if (isChaged)
    {
        WriteLock(&g_control_code_rwlock);
        printf("g_control_code가 %d에서 %d로 바뀝니다.\n", g_control_code, code);
        g_control_code = code;
        WriteUnLock(&g_control_code_rwlock);
    }
}

char* RegistClient(int clientSocket)
{   
    char* nowClientID = malloc(sizeof(char) * CLIENT_NAME_SIZE);
    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    readSize = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (readSize == -1)
    {
        fprintf(stderr, "read 에러\n");
        return NULL;
    }
    buffer[readSize] = '\0';
    printf("클라이언트로부터 클라이언트 정보를 받았습니다\n%s\n", buffer);

    cJSON* json = cJSON_Parse(buffer);
    char* clientID = cJSON_GetObjectItem(json, "clientID")->valuestring;
    char* species = cJSON_GetObjectItem(json, "Species")->valuestring;

    Color color;
    cJSON* jsonColor = cJSON_GetObjectItem(json, "Color");

    color.Red = cJSON_GetObjectItem(jsonColor, "Red")->valueint;
    color.Green = cJSON_GetObjectItem(jsonColor, "Green")->valueint;
    color.Blue = cJSON_GetObjectItem(jsonColor, "Blue")->valueint;

    char* path = GetLogDirPath();
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
        if (strcmp(csv->data[i][CLIENT_LIST_CSV_CLIENT_ID], clientID) == 0)
        {
            isExist = 1;
            break;
        }
    }
    DestroyCSV(csv);

    if(!isExist)
    {
        printf("클라이언트 정보가 없습니다. 클라이언트(%s)를 등록합니다.", clientID);
        if((color.Red == 0) && (color.Green == 0) && (color.Blue == 0))
        {
            if (strcmp(species, CHEERY_BLOSSOM) == 0)
            {  
                color = GetDefalutTargetColor(SPECIES_CHERRY_BLOSSOM);
            }
            else if (strcmp(species, MAPLE) == 0)
            {
                color = GetDefalutTargetColor(SPECIES_MAPLE);
            }
            else
            {
                ASSERT(0, "올바르지 않은 종 이름");
            }
        }
        fprintf(clientList, "%s,%s,%d,%d,%d\n", clientID, species, color.Red, color.Green, color.Blue);
        printf("클라이언트가 새로 등록되었습니다. : %s\n", clientID);
    }
    strcpy(nowClientID, clientID);

    cJSON_Delete(json);
    free(path);
    fclose(clientList);
    return nowClientID;
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
    
    SensorData* sensorData = malloc(sizeof(SensorData));
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        ASSERT(0, "Failed to parse json");
        return NULL;
    }

    cJSON* senSorDatajson = cJSON_GetObjectItem(json, "SensorData");

    sensorData->flame = cJSON_GetObjectItem(senSorDatajson, "Flame")->valueint;
    sensorData->gas = cJSON_GetObjectItem(senSorDatajson, "Gas")->valueint;
    sensorData->humidity = cJSON_GetObjectItem(senSorDatajson, "Humidity")->valueint;
    sensorData->light = cJSON_GetObjectItem(senSorDatajson, "Light")->valueint;
    sensorData->temp = cJSON_GetObjectItem(senSorDatajson, "Temp")->valueint;

    cJSON* jsonColor = cJSON_GetObjectItem(senSorDatajson, "Color");
    sensorData->color.Red = cJSON_GetObjectItem(jsonColor, "Red")->valueint;
    sensorData->color.Green = cJSON_GetObjectItem(jsonColor, "Green")->valueint;
    sensorData->color.Blue = cJSON_GetObjectItem(jsonColor, "Blue")->valueint;

    printf("%d, %d\n", sensorData->light, sensorData->color.Green);
    return sensorData;
}

Color GetRegisteredColor(char* clientID)
{
    Color color = {0, 0, 0};

    char* path = GetLogDirPath();
    strcat(path, CLIENT_LIST_PATH);
    FILE* clientList = fopen(path, "r");
    if (clientList == NULL)
    {
        perror("fopen");
        return color;
    }

    CSV* registeredData = ParseCSV(clientList);
    free(path);
    fclose(clientList);

    for (int i = 0; i < registeredData->csvSize; i++)
    {
        printf("%s, %s\n", registeredData->data[i][CLIENT_LIST_CSV_CLIENT_ID], clientID);
        if (strcmp(registeredData->data[i][CLIENT_LIST_CSV_CLIENT_ID], clientID) == 0)
        {
            color.Red = atoi(registeredData->data[i][CLIENT_LIST_CSV_COLOR_RED]);
            color.Green = atoi(registeredData->data[i][CLIENT_LIST_CSV_COLOR_GREEN]);
            color.Blue = atoi(registeredData->data[i][CLIENT_LIST_CSV_COLOR_BLUE]);
        }
    }
    ASSERT((color.Red | color.Green | color.Blue), "등록되지 않은 클라이언트\n");

    DestroyCSV(registeredData);
    return color;
}

void* DMonitorThreadAction(void* arg)
{
    int clientSocket = *(int*)arg;
    char* clientID = RegistClient(clientSocket);
    TotalData* totalData = InitTotalData();

    int isLEDSelected = 0;
    Color color = GetRegisteredColor(clientID);
    int colorDistance = -1;

    unsigned char controlCode;

    time_t currentTime;
    struct tm localTime;
    int currentDay;

    time(&currentTime);
    localtime_r(&currentTime, &localTime);
    currentDay = localTime.tm_mday;

    while(1)
    {
        time(&currentTime);
        localtime_r(&currentTime, &localTime);
        if (currentDay != localTime.tm_mday)
        {
            ProcessDataLog(clientID, totalData);
            currentDay = localTime.tm_mday;
        }

        SensorData* sensorData = ReadSensorData(clientSocket);
        if (sensorData == NULL)
        {
            break;
        }
        totalData->sensorData[totalData->count++] = sensorData;

        //   TODO : 실제로 불이 나는 조건으로 변경
        if (sensorData->flame && sensorData->gas)
        {
            controlCode |= EMERGENCY | IS_CHANGED | BUZZER;
        }

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

        // TODO : 테스트 필요
        if (isLEDSelected)
        {   
            int processLEDPercentage = 0;
            ReadLock(&g_color_rwlock);
            if ((g_color.Red == 0) | (g_color.Green == 0) | (g_color.Blue == 0))
            {
                processLEDPercentage = GetColorDistance(g_color, sensorData->color);
            }
            else
            {
                processLEDPercentage = GetColorDistance(GetRegisteredColor(clientID), sensorData->color);
            }
            ReadUnlock(&g_color_rwlock);
            
            controlCode |= IS_CHANGED;
            controlCode &= ~(LED_RED | LED_GREEN | LED_BLUE);
            if (processLEDPercentage > 0 && 20 > processLEDPercentage)
            {
                controlCode |= LED_BLUE;
            }
            else if (processLEDPercentage >= 20 && 80 > processLEDPercentage)
            {
                controlCode |= LED_GREEN;
            }
            else if (processLEDPercentage >= 80 && 100 > processLEDPercentage)
            {
                controlCode |= LED_RED;
            }
        }

        // TODO : 테스트 필요
        SetupControlCode(controlCode);
        controlCode = 0;
        sleep(1);
    }
    free(clientID);
    close(clientSocket);
    exit(1);
}

void DMonitorThreadCreate(int clientSocket)
{
    pthread_t threadID = 0;

    int* _clientSocket = malloc(sizeof(int));
    *_clientSocket = clientSocket;

    pthread_create(&threadID, NULL, DMonitorThreadAction, (void*)_clientSocket);
    pthread_detach(threadID);
}
