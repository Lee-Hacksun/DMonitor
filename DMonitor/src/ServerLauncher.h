#ifndef SERVERLAUNCHER_H
#define SERVERLAUNCHER_H

#include <netinet/in.h>
#include <sys/epoll.h>

#define PORT 5000
#define LISTEN_QUEUE_SIZE 5
#define EPOLL_SIZE 10

typedef struct _EventPolling
{
    struct epoll_event* events;
    struct epoll_event event;
    int eventPollingFD;
    int evnetCount;
} EventPolling;


void InitServerSocket(int* serverSocket, struct sockaddr_in* serverAddress);
void InitEventPolling(EventPolling* eventPolling);
void AddEventPolling(EventPolling* eventPolling, int FileDescriptor);
void DestroyEventPolling(EventPolling* eventPolling);

int LaunchServer();
void AcceptMultiClient(int serverSocket);

#endif