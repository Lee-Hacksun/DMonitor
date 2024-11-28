#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ClientManager.h"
#include "DMonitorThread.h"

int clientCount = 0;
int clientSockets[MAX_CONNECTION];
pthread_mutex_t mutex;

void* DMonitorThreadAction(void* arg)
{
    while(1)
    {
        FILE* log = fopen("test.txt", "a");
        ReadLock(&g_rwlock);
        fprintf(log, "%d:%d:%d\n", g_color.Red, g_color.Green, g_color.Blue);
        ReadUnlock(&g_rwlock);
        fflush(log);
        sleep(5);

        // 초기화, 클라이언트 아이디 가져오기
        // 여러 값 비교 및, 해당하는 값 있으면 실행
    }

    close(*(int*)arg);
    free(arg);
}

void InitDMonitorThread()
{
    pthread_mutex_init(&mutex, NULL);
}

void DMonitorThreadCreate(int clientSocket)
{
    pthread_t threadID = 0;
    
    // pthread_mutex_lock(&mutex);
    // clientSockets[clientCount++] = clientSocket;
    // pthread_mutex_unlock(&mutex);

    int* _clientSocket = malloc(sizeof(int));
    *_clientSocket = clientSocket;

    pthread_create(&threadID, NULL, DMonitorThreadAction, (void*)_clientSocket);
    pthread_detach(threadID);
}
