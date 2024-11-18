#ifndef SERVERLAUNCHER_H
#define SERVERLAUNCHER_H

#include <netinet/in.h>

#define PORT 5000

void InitServerSocket(int* serverSocket, struct sockaddr_in* serverAddress);
int LaunchServer();
void AcceptMultiClient(int serverSocket);

#endif