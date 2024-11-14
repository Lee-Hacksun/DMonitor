#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "DMonitorThread.h"

int clientCount = 0;
int clientSockets[MAX_CONNECTION];
pthread_mutex_t mutex;

void* DMonitorThreadAction(void* arg)
{
    int client = *((int*)arg);
    printf("arg is : %d\n", client);

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
