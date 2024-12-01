#include <libgen.h>
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
#include "RWLock.h"
#include "ServerLauncher.h"

#pragma region ThreadVariable

// TODO : 뮤텍스 사용 변수
// LinkedList fireDetectClientID;

RWLock g_color_rwlock;
Color g_color = 
{
0, 0, 0
};
char g_colorClientID[BUFFER_SIZE] = "";
char EXEPath[PATH_MAX];

#pragma endregion

void SetupColor(char* clientID, Color color)
{
    strcpy(g_colorClientID, clientID);

    g_color.Red = color.Red;
    g_color.Green = color.Green;
    g_color.Blue = color.Blue;
}

void SetEXEPath()
{
    ssize_t len = readlink("/proc/self/exe", EXEPath, sizeof(EXEPath));

    if (len != -1)
    {
        EXEPath[len] = '\0';
        strcpy(EXEPath,dirname(EXEPath));
        printf("Setup EXEPath : %s\n", EXEPath);
    }
    else
    {
        perror("readlink");
    }
}

void CheckWorkingDirectory()
{
    char logPath[PATH_MAX];
    char* logDir = "/log";
    
    ASSERT(strlen(EXEPath) + strlen("/log") < PATH_MAX, "경로의 길이가 너무 깁니다.\n");
    snprintf(logPath, sizeof(logPath), "%s%s", EXEPath, logDir);
    
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
}

void CheckClientListFile()
{
    char path[PATH_MAX];
    strcpy(path, EXEPath);
    strcat(path, CLIENT_LIST_PATH);
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
        fprintf(clientList, "clientID,Hello\n");
    }

    fclose(clientList);
}

void RunClientManager(int inputPipe)
{  
    int serverSocket = LaunchServer();

    InitRWLock(&g_color_rwlock);
    SetEXEPath();
    CheckWorkingDirectory();
    CheckClientListFile();

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

                    if (readSize > 1)
                    {
                        Color color;
                        
                        buffer[readSize] = '\0';

                        cJSON* json = cJSON_Parse(buffer);
                        if (json == NULL)
                        {
                            ASSERT(0, "Failed to parse json");
                            continue;
                        }

                        char* clientName = cJSON_GetObjectItem(json, "clientID")->valuestring;
                        if (clientName == NULL)
                        {
                            ASSERT(0, "Failed to parse json");
                            continue;
                        }

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
                    }
                }
            }
        }
    }
    DestroyEventPolling(&eventPolling);
    close(serverSocket);
}