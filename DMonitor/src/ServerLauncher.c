#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h> 

#include "DMonitorThread.h"
#include "ServerLauncher.h"

void InitServerSocket(int* serverSocket, struct sockaddr_in* serverAddress)
{
    *serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    memset(serverAddress, 0, sizeof(*serverAddress));
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress->sin_port = htons(PORT);
}

void InitEventPolling(EventPolling* eventPolling)
{
    eventPolling->eventPollingFD = epoll_create(EPOLL_SIZE);
    eventPolling->events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
}

void AddEventPolling(EventPolling* eventPolling, int FileDescriptor)
{
    eventPolling->event.events = EPOLLIN;
    eventPolling->event.data.fd = FileDescriptor;
    epoll_ctl(eventPolling->eventPollingFD, EPOLL_CTL_ADD, FileDescriptor, &eventPolling->event);
}

void DestroyEventPolling(EventPolling* eventPolling)
{
    free(eventPolling->events);
    close(eventPolling->eventPollingFD);
}

int LaunchServer()
{
    int serverSocket = 0;
    struct sockaddr_in serverAddress;

    InitServerSocket(&serverSocket, &serverAddress);

    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress))==-1)
	{
        // TODO : 에러 핸들링 추가
    }

    if(listen(serverSocket, LISTEN_QUEUE_SIZE)==-1)
	{
        // TODO : 에러 핸들링 추가
    }

    return serverSocket;
}

// TODO : Accept 로그 기록 
void AcceptMultiClient(int serverSocket)
{
    int clientSocket = 0;
    struct sockaddr_in clientAddress;

    int clientAddressSize = sizeof(clientAddress);
    if((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize)) < 0)
    {
        // TODO : 에러 핸들링 추가
        exit(EXIT_FAILURE);
    }
    DMonitorThreadCreate(clientSocket);
}