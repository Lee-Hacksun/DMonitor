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
#include "logger.h"

#pragma region ThreadVariable

RWLock g_color_rwlock;
Color g_color = {0, 0, 0};
char g_colorClientID[BUFFER_SIZE] = "";

RWLock g_control_code_rwlock;
unsigned char g_control_code = 0;

pthread_mutex_t  g_client_list_lock;
pthread_mutex_t  g_sensor_lock;
pthread_mutex_t  g_progress_lock;

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

    if (stat(logPath, &st) == -1)
    {
        if (mkdir(logPath, 0700) != 0)
        {
            perror("작업폴더 생성 실패");
        }
        else 
        {
        }
    }
    else 
    {
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
        LogPrintf("%s 파일이 없습니다. 클라이언트 리스트 파일을 생성합니다.\n", path);
        clientList = fopen(path, "w");
        if (clientList == NULL)
        {
            perror("fopen");
        }

        // TODO : 초기값 설정
        fprintf(clientList, CLIENT_LIST_CSV_HEADER);
        
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
        LogPrintf("%s 파일이 없습니다. 센서 파일을 생성합니다.\n", path);
        sensor = fopen(path, "w");
        if (sensor == NULL)
        {
            perror("fopen");
        }

        fprintf(sensor, SENSOR_CSV_HEADER);
    }

    fclose(sensor);
    pthread_mutex_unlock(&g_sensor_lock);

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
        LogPrintf("%s 파일이 없습니다. 프로그레스 파일을 생성합니다.\n", path);
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
    CheckWorkingDirectory();

    char* path = GetLogDirPath();
    strcat(path, LOG_FILE_PATH);
    InitLogger(path);
    free(path);

    int serverSocket = LaunchServer();
    int clientSocket = AcceptClient(serverSocket);

    InitRWLock(&g_color_rwlock);
    InitRWLock(&g_control_code_rwlock);

    pthread_mutex_init(&g_client_list_lock, NULL);
    pthread_mutex_init(&g_sensor_lock, NULL);
    pthread_mutex_init(&g_progress_lock, NULL);

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
        eventPolling.evnetCount = epoll_wait(eventPolling.eventPollingFD, eventPolling.events, EPOLL_SIZE, 5000);
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
                    LogPrintf("UI 프로세스로부터 데이터를 받았습니다 : \n%s\n", buffer);

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
                        LogPrintf("g_color값을 변경했습니다.(%d, %d, %d)\n g_clientID값을 변경했습니다.(%s)\n", color.Red, color.Green, color.Blue, clientName);
                        WriteUnLock(&g_color_rwlock);
                    }
                    else // controlCode setting
                    {
                        int controlCode = cJSON_GetObjectItem(json, "BuzzerOff")->valueint;

                        if (controlCode == 1)
                        {
                            WriteLock(&g_control_code_rwlock);
                            unsigned  char old_g_control_code = g_control_code;
                            g_control_code &= (LED_RED | LED_GREEN | LED_BLUE);
                            LogPrintf("controlCode를 %d에서 %d(으)로 변경합니다.\n", old_g_control_code, g_control_code);
                            WriteUnLock(&g_control_code_rwlock);
                        }
                    }    
                    cJSON_Delete(json);          
                }
            }
        }
        // TODO : 테스트
        cJSON* json = cJSON_CreateObject();

        ReadLock(&g_control_code_rwlock);
        cJSON_AddNumberToObject(json, "ControlCode", g_control_code);

        char* jsonString = cJSON_Print(json);
        ssize_t writeSize = write(clientSocket, jsonString, strlen(jsonString));
        LogPrintf("클라이언트에게 controlCode(%d)를 보냅니다.\n데이터 : \n%s\n", g_control_code, jsonString);
        ReadUnlock(&g_control_code_rwlock);
        cJSON_Delete(json);
    }
    DestroyEventPolling(&eventPolling);
    DestroyRWLock(&g_color_rwlock);
    DestroyRWLock(&g_control_code_rwlock);

    pthread_mutex_destroy(&g_client_list_lock);
    pthread_mutex_destroy(&g_sensor_lock);
    pthread_mutex_destroy(&g_progress_lock);
    
    close(serverSocket);
}