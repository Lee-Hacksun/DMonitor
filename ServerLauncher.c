#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

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

int LaunchServer()
{
    int serverSocket = 0;
    struct sockaddr_in serverAddress;

    InitServerSocket(&serverSocket, &serverAddress);

    if(bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress))==-1)
	{
        // TODO : 에러 핸들링 추가
    }

    if(listen(serverSocket, 5)==-1)
	{
        // TODO : 에러 핸들링 추가
    }

    return serverSocket;
}

// TODO : Accept 로그 기록 
void AcceptMultiClient(int serverSocket, void* (*threadAction)(void*))
{
    struct sockaddr_in clientAddress;

    // InitDMonitorThread();

    while(1)
    {
        int clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
        DMonitorThreadCreate(clientSocket);
    }
}