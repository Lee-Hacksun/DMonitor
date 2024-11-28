#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

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

RWLock g_rwlock;
Color g_color = 
{
0, 0, 0
};
char g_colorClientID[BUFFER_SIZE] = "";

#pragma endregion

void SetupColor(char* clientID, Color color)
{
    strcpy(g_colorClientID, clientID);

    g_color.Red = color.Red;
    g_color.Green = color.Green;
    g_color.Blue = color.Blue;
}

void RunClientManager(int inputPipe)
{  
    int serverSocket = LaunchServer();

    InitRWLock(&g_rwlock);

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

                        WriteLock(&g_rwlock);
                        SetupColor(clientName, color);
                        WriteUnLock(&g_rwlock);
                    }
                }
            }
        }
    }
    DestroyEventPolling(&eventPolling);
    close(serverSocket);
}