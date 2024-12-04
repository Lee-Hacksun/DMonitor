#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cjson/cJSON.h"

#include "Color.h"
#include "ClientManager.h"
#include "DebugUtil.h"
#include "DMonitorThread.h"
#include "FileManager.h"
#include "FileLock.h"
#include "RWLock.h"
#include "ServerLauncher.h"

#pragma region ThreadVariable

// TODO : 뮤텍스 사용 변수
// LinkedList fireDetectClientID;

RWLock g_color_rwlock;
Color g_color = {0, 0, 0};
char g_colorClientID[BUFFER_SIZE] = "";

RWLock g_control_code_rwlock;
unsigned char g_control_code = 0;

#pragma endregion

void SetupColor(char* clientID, Color color)
{
    strcpy(g_colorClientID, clientID);

    g_color.Red = color.Red;
    g_color.Green = color.Green;
    g_color.Blue = color.Blue;
}

void CheckWorkingDirectory()
{
    char* logPath = GetLogDirPath();

    struct stat st = {0};

    printf("%s 작업폴더 존재 확인\n", logPath);
    if (stat(logPath, &st) == -1)
    {
        printf("작업폴더 존재하지 않습니다.\n");
        if (mkdir(logPath, 0700) != 0)
        {
            perror("작업폴더 생성 실패");
        }
        else 
        {
            printf("작업폴더를 생성했습니다.\n");
        }
    }
    else 
    {
        printf("작업폴더가 존재합니다.\n");
    }

    free(logPath);
}

void CheckClientListFile()
{
    char* path = GetLogDirPath();
    strcat(path, CLIENT_LIST_PATH);

    pthread_mutex_lock(&g_client_list_lock);
    FILE* clientList = fopen(path, "r");

    if (clientList == NULL)
    {
        printf("%s 파일이 없습니다. 클라이언트 리스트 파일을 생성합니다.\n", path);
        clientList = fopen(path, "w");
        if (clientList == NULL)
        {
            perror("fopen");
        }

        // TODO : 초기값 설정
        fprintf(clientList, "clientID,Species,Red,Green,Blue,RegionCode\n");
    }

    fclose(clientList);
    pthread_mutex_unlock(&g_client_list_lock);

    free(path);
}

void CheckSensorFile()
{
    char* path = GetLogDirPath();
    strcat(path, SENSOR_PATH);

    pthread_mutex_lock(&g_sensor_lock);
    FILE* sensor = fopen(path, "r");

    if (sensor == NULL)
    {
        printf("%s 파일이 없습니다. 센서 파일을 생성합니다.\n", path);
        sensor = fopen(path, "w");
        if (sensor == NULL)
        {
            perror("fopen");
        }

        fprintf(sensor, SENSOR_CSV_HEADER);
    }

    fclose(sensor);
    pthread_mutex_unlock(&g_client_list_lock);

    free(path);
}

void CheckProgressFile()
{
    char* path = GetLogDirPath();
    strcat(path, PROGRESS_LIST_PATH);

    pthread_mutex_lock(&g_progress_lock);
    FILE* progress = fopen(path, "r");

    if (progress == NULL)
    {
        printf("%s 파일이 없습니다. 프로그레스 파일을 생성합니다.\n", path);
        progress = fopen(path, "w");
        if (progress == NULL)
        {
            perror("fopen");
        }

        fprintf(progress, PROGRESS_LIST_CSV_HEADER);
    }

    fclose(progress);
    pthread_mutex_unlock(&g_progress_lock);

    free(path);
}

void RunClientManager(int inputPipe)
{  
    int serverSocket = LaunchServer();
    int clientSocket = AcceptClient(serverSocket);

    InitRWLock(&g_color_rwlock);
    InitRWLock(&g_control_code_rwlock);

    CheckWorkingDirectory();
    CheckClientListFile();
    CheckSensorFile();
    CheckProgressFile();

    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    EventPolling eventPolling;
    InitEventPolling(&eventPolling);
    AddEventPolling(&eventPolling, serverSocket);
    AddEventPolling(&eventPolling, inputPipe);

    while(1)
    {
        eventPolling.evnetCount = epoll_wait(eventPolling.eventPollingFD, eventPolling.events, EPOLL_SIZE, 1000);
        if (eventPolling.evnetCount == -1)
        {
            perror("epoll_wait() faild");
            exit(1);
        }
        else if (eventPolling.evnetCount != 0)
        {
            for (int i = 0; i < eventPolling.evnetCount; i++)
            {
                if (eventPolling.events[i].data.fd == serverSocket)
                {
                    AcceptMultiClient(serverSocket);
                }

                if (eventPolling.events[i].data.fd == inputPipe)
                {
                    readSize = read(inputPipe, buffer, sizeof(buffer) - 1);
                    if (readSize < 0)
                    {
                        perror("read Error!");
                        continue;
                    }                        
                    buffer[readSize] = '\0';

                    cJSON* json = cJSON_Parse(buffer);
                    if (json == NULL)
                    {
                        ASSERT(0, "Failed to parse json");
                        continue;
                    }

                    cJSON* cilentNameJson = cJSON_GetObjectItem(json, "clientID");

                    // color setting
                    if (cilentNameJson != NULL)
                    {
                        Color color;

                        char* clientName = cilentNameJson->valuestring;

                        cJSON* jsonColor = cJSON_GetObjectItem(json, "color");
                        if (jsonColor == NULL)
                        {
                            ASSERT(0, "Failed to parse json");
                            continue;
                        }

                        color.Red = cJSON_GetObjectItem(jsonColor, "Red")->valueint;
                        color.Green = cJSON_GetObjectItem(jsonColor, "Green")->valueint;
                        color.Blue = cJSON_GetObjectItem(jsonColor, "Blue")->valueint;

                        WriteLock(&g_color_rwlock);
                        SetupColor(clientName, color);
                        WriteUnLock(&g_color_rwlock);

                        cJSON_Delete(json);
                    }
                    else // controlCode setting
                    {
                        int controlCode = cJSON_GetObjectItem(json, "BuzzerOff")->valueint;

                        if (controlCode == 1)
                        {
                            WriteLock(&g_control_code_rwlock);
                            g_control_code &= (LED_RED | LED_GREEN | LED_BLUE);
                            WriteUnLock(&g_control_code_rwlock);
                        }

                        cJSON_Delete(json);
                    }
                    cJSON_Delete(cilentNameJson);                    
                }
            }
        }
        // TODO : 테스트
        cJSON* json = cJSON_CreateObject();

        ReadLock(&g_control_code_rwlock);
        cJSON_AddNumberToObject(json, "ControlCode", g_control_code);
        ReadUnlock(&g_control_code_rwlock);

        char* jsonString = cJSON_Print(json);
        write(clientSocket, jsonString, strlen(jsonString));
        cJSON_Delete(json);
    }
    DestroyEventPolling(&eventPolling);
    DestroyRWLock(&g_color_rwlock);
    DestroyRWLock(&g_control_code_rwlock);
    close(serverSocket);
}