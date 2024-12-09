#include <math.h>
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
#include "FileManager.h"
#include "FileLock.h"
#include "SensorData.h"
#include "logger.h"

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

// TODO : 테스트 해야댐 
void UpdateProgress(char* clientID, int progress)
{   
    char* path = GetLogDirPath();
    strcat(path, PROGRESS_LIST_PATH);

    pthread_mutex_lock(&g_progress_lock);
    FILE* progressFile = fopen(path, "r");
    if (progressFile == NULL)
    {
        perror("fopen");
        return;
    }

    CSV* csv = ParseCSV(progressFile, NULL, NULL); 
    fclose(progressFile);
    pthread_mutex_unlock(&g_progress_lock);

    for (int i = 0; i < csv->csvSize; i++)
    {
        if (strcmp(csv->data[i][PROGRESS_LIST_CSV_CLIENT_ID], clientID) == 0)
        {
            free(csv->data[i][PROGRESS_LIST_CSV_PROGRESS]);
            csv->data[i][PROGRESS_LIST_CSV_PROGRESS] = malloc(sizeof(char) * 3);
            sprintf(csv->data[i][PROGRESS_LIST_CSV_PROGRESS], "%d", progress);
        }
    } 

    pthread_mutex_lock(&g_progress_lock);
    progressFile = fopen(path, "w");
    if (progressFile == NULL)
    {
        perror("fopen");
        return;
    }

    CSVWrite(progressFile, csv, PROGRESS_LIST_CSV_HEADER);
    fclose(progressFile);
    pthread_mutex_unlock(&g_progress_lock);

    free(path);
}

// TODO : 테스트 필요
void ProcessDataLog(char* clientID, TotalData* totalData)
{
    int avsFlame = 0;
    int avgGas = 0;
    float avgHumidity = 0;
    int avgLigh = 0;
    float avgTemp = 0;
    int avgRed = 0;
    int avgGreen = 0;
    int avgBlue = 0;
        
    for (int i = 0; i < totalData->count; i++)
    {
        avsFlame += totalData->sensorData[i]->flame;
        avgGas += totalData->sensorData[i]->gas;
        avgHumidity += totalData->sensorData[i]->humidity;
        avgLigh += totalData->sensorData[i]->light;
        avgTemp += totalData->sensorData[i]->temp;

        avgRed += totalData->sensorData[i]->color.Red;
        avgGreen += totalData->sensorData[i]->color.Green;
        avgBlue += totalData->sensorData[i]->color.Blue;
    }

    avsFlame /= totalData->count;
    avgGas /= totalData->count;
    avgHumidity /= totalData->count;
    avgLigh /= totalData->count;
    avgTemp /= totalData->count;

    avgRed /= totalData->count;
    avgGreen /= totalData->count;
    avgBlue /= totalData->count;

    char* path = GetLogDirPath();
    strcat(path, SENSOR_PATH);

if (pthread_mutex_trylock(&g_sensor_lock) == 0) {
    // 잠금이 성공한 경우
    LogPrintf("Lock acquired successfully!\n");
    pthread_mutex_unlock(&g_sensor_lock); // 작업 끝나면 잠금 해제
} else {
    // 잠금이 실패한 경우
    LogPrintf("Lock is already held by another thread.\n");
}

    pthread_mutex_lock(&g_sensor_lock);

    FILE* sensorLog = fopen(path, "a+");
    if (sensorLog == NULL)
    {
        perror("fopen");
    }

    Color color;
    color.Red = avgRed;
    color.Green = avgGreen;
    color.Blue = avgBlue;

    int progress = GetColorDistance(GetRegisteredColor(clientID), color);

    fprintf(sensorLog, "%s,%d,%d,%.2f,%d,%.2f,%d,%d,%d,%d\n",
    clientID, 
    (int)avsFlame, 
    (int)avgGas, 
    (float)avgHumidity, 
    (int)avgLigh, 
    (float)avgTemp, 
    (int)color.Red, 
    (int)color.Green, 
    (int)color.Blue,
    progress);

    UpdateProgress(clientID, progress);

    LogPrintf("클라이언트(%s)의 센서 데이터와 진행률 데이터를 갱신했습니다.\n", clientID);

    fclose(sensorLog);
    pthread_mutex_unlock(&g_sensor_lock);

    for (int i = 0; i < totalData->count; i++)
    {
        free(totalData->sensorData[i]);
    }
    totalData->count = 0;
    free(path);
}

void SetupControlCode(unsigned char code)
{
    int isChaged = 0;

    ReadLock(&g_control_code_rwlock);
    if (code & IS_SUPER_DO)
    {
        isChaged = 1;
    }
    else if ((code & EMERGENCY) && (g_control_code & IS_SUPER_DO) == 0)
    {
        isChaged = 1;
    }
    else if ((code & IS_CHANGED) && ((g_control_code & (IS_SUPER_DO | EMERGENCY)) == 0))
    { 
        isChaged = 1;
    }
    ReadUnlock(&g_control_code_rwlock);

    if (isChaged)
    {
        WriteLock(&g_control_code_rwlock);
        if (g_control_code != code)
        {
            g_control_code = code;
            LogPrintf("g_control_code가 %d에서 %d으로 바뀝니다.\n", g_control_code, code);
        }
        WriteUnLock(&g_control_code_rwlock);
    }
}

char* RegistClient(int clientSocket)
{   
    char* nowClientID = malloc(sizeof(char) * CLIENT_ID_SIZE);
    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    readSize = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (readSize == -1)
    {
        fprintf(stderr, "read 에러\n");
        return NULL;
    }
    buffer[readSize] = '\0';
    LogPrintf("클라이언트(%d)로부터 클라이언트 등록 정보를 받았습니다\n%s\n", clientSocket, buffer);

    cJSON* json = cJSON_Parse(buffer);
    char* clientID = cJSON_GetObjectItem(json, "clientID")->valuestring;
    char* species = cJSON_GetObjectItem(json, "Species")->valuestring;
    char* regionCode = cJSON_GetObjectItem(json, "RegionCode")->valuestring;

    Color color;
    cJSON* jsonColor = cJSON_GetObjectItem(json, "Color");

    color.Red = cJSON_GetObjectItem(jsonColor, "Red")->valueint;
    color.Green = cJSON_GetObjectItem(jsonColor, "Green")->valueint;
    color.Blue = cJSON_GetObjectItem(jsonColor, "Blue")->valueint;

    char* path = GetLogDirPath();
    strcat(path, CLIENT_LIST_PATH);

    pthread_mutex_lock(&g_client_list_lock);
    FILE* clientList = fopen(path, "a+");
    if (clientList == NULL)
    {
        perror("fopen");
        return NULL;
    }

    int isExist = 0;
    CSV* csv = ParseCSV(clientList, NULL, NULL);
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
        LogPrintf("클라이언트 정보가 없습니다. 클라이언트(%s)를 등록합니다.\n", clientID);
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
        char* progPath = GetLogDirPath();
        strcat(progPath, PROGRESS_LIST_PATH);
        FILE* progreeList = fopen(progPath, "a+");
        {
            if (progreeList == NULL)
            {
                perror("fopen");
            }
        }

        fprintf(clientList, "%s,%s,%d,%d,%d,%s\n", clientID, species, color.Red, color.Green, color.Blue, regionCode);
        fprintf(progreeList, "%s,0", clientID);
        fclose(progreeList);

        LogPrintf("클라이언트가 새로 등록되었습니다. : %s\n", clientID);
    }
    strcpy(nowClientID, clientID);

    cJSON_Delete(json);
    free(path);
    fclose(clientList);
    pthread_mutex_unlock(&g_client_list_lock);

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
        LogPrintf("클라이언트(%d)가 연결을 종료했습니다.\n", clientSocket);
        return NULL;
    }
    buffer[readSize] = '\0';
    LogPrintf("클라이언트(%d)로부터 받은 센서 데이터: %s\n", clientSocket, buffer);
    
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
    sensorData->humidity = (float)cJSON_GetObjectItem(senSorDatajson, "Humidity")->valuedouble;
    sensorData->light = cJSON_GetObjectItem(senSorDatajson, "Light")->valueint;
    sensorData->temp = (float)cJSON_GetObjectItem(senSorDatajson, "Temperature")->valuedouble;

    cJSON* jsonColor = cJSON_GetObjectItem(senSorDatajson, "Color");
    sensorData->color.Red = cJSON_GetObjectItem(jsonColor, "Red")->valueint;
    sensorData->color.Green = cJSON_GetObjectItem(jsonColor, "Green")->valueint;
    sensorData->color.Blue = cJSON_GetObjectItem(jsonColor, "Blue")->valueint;

    return sensorData;
}

Color GetRegisteredColor(char* clientID)
{
    Color color = {0, 0, 0};

    char* path = GetLogDirPath();
    strcat(path, CLIENT_LIST_PATH);

    pthread_mutex_lock(&g_client_list_lock);
    FILE* clientList = fopen(path, "r");
    if (clientList == NULL)
    {
        perror("fopen");
        return color;
    }

    int index = CLIENT_LIST_CSV_CLIENT_ID;
    CSV* registeredData = ParseCSVOption(clientList, Select, &index, clientID, NULL, NULL);
    free(path);
    fclose(clientList);
    pthread_mutex_unlock(&g_client_list_lock);

    if (registeredData == NULL)
    {
        ASSERT(0, "등록되지 않은 클라이언트\n");
    }
    else
    {
        color.Red = atoi(registeredData->data[0][CLIENT_LIST_CSV_COLOR_RED]);
        color.Green = atoi(registeredData->data[0][CLIENT_LIST_CSV_COLOR_GREEN]);
        color.Blue = atoi(registeredData->data[0][CLIENT_LIST_CSV_COLOR_BLUE]);
    }

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
    unsigned char controlCode = 0;

    time_t currentTime;
    struct tm localTime;
    int currentDay;
    int currentMin;

    time(&currentTime);
    localtime_r(&currentTime, &localTime);
    currentDay = localTime.tm_mday;
    currentMin = localTime.tm_min;
    while(1)
    {
        time(&currentTime);
        localtime_r(&currentTime, &localTime);
        #if !TEST
        {
            if (currentDay != localTime.tm_mday)
            {
                ProcessDataLog(clientID, totalData);
                currentDay = localTime.tm_mday;
            }
        }
        #elif TEST
        {
            if (currentMin != localTime.tm_min)
            {
                ProcessDataLog(clientID, totalData);
                currentMin = localTime.tm_min;
            }
        }
        #endif

        SensorData* sensorData = ReadSensorData(clientSocket);
        if (sensorData == NULL)
        {
            break;
        }
        totalData->sensorData[totalData->count++] = sensorData;

        if (sensorData->flame == 1)
        {
            controlCode |= EMERGENCY | IS_CHANGED | BUZZER;
            LogPrintf("클라이언트(%s)에서 화재를 감지했습니다. \n제어코드를 %d로 변경했습니다.\n", clientID, controlCode);
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
            LogPrintf("[ %s ] 클라이언트가 선택되었습니다. 해당 클라이언트의 센서 정보를 추적합니다.\n", clientID);

            int processLEDPercentage = 0;
            ReadLock(&g_color_rwlock);
            if ((g_color.Red == 0) && (g_color.Green == 0) && (g_color.Blue == 0))
            {
                processLEDPercentage = GetColorDistance(GetRegisteredColor(clientID), sensorData->color);
            }
            else
            {
                processLEDPercentage = GetColorDistance(g_color, sensorData->color);
            }
            ReadUnlock(&g_color_rwlock);
            
            controlCode |= IS_CHANGED;
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
        
        SetupControlCode(controlCode);        
        controlCode = 0;        

        sleep(1);
    }
    free(clientID);
    close(clientSocket);
}

void DMonitorThreadCreate(int clientSocket)
{
    pthread_t threadID = 0;

    int* _clientSocket = malloc(sizeof(int));
    *_clientSocket = clientSocket;

    pthread_create(&threadID, NULL, DMonitorThreadAction, (void*)_clientSocket);
    pthread_detach(threadID);

    LogPrintf("센서 클라이언트(%d)와 연결을 성공했습니다.\n", clientSocket);
}
