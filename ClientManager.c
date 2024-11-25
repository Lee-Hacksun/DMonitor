#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "cjson/cJSON.h"

#include "Color.h"
#include "ClientManager.h"
#include "DebugUtil.h"
#include "DMonitorThread.h"
#include "ServerLauncher.h"

#pragma region ThreadVariable

pthread_rwlock_t  g_RWLock;

// TODO : 뮤텍스 사용 변수
// LinkedList fireDetectClientID;

// TODO : RWLock 사용 변수
Color g_color;
char* g_colorClientID;

#pragma endregion

void SetupColor(char* clientID, Color color)
{

}

void RunClientManager(int inputPipe)
{  
    int serverSocket = LaunchServer();

    char buffer[BUFFER_SIZE];
    ssize_t readSize;

    pthread_rwlock_init(&g_RWLock, NULL);

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
                    FILE* pipe = fopen("test.txt", "a");
                    Color color;

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


                    fprintf(pipe, "clientID: %s\n Color: %d, %d, %d\n", clientName, color.Red, color.Green, color.Blue);
                    fclose(pipe);
                }
            }
        }
    }
    DestroyEventPolling(&eventPolling);
    close(serverSocket);
}