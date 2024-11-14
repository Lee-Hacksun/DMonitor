#include <stdlib.h>
#include <pthread.h>

#include "ClientManager.h"
#include "DMonitorThread.h"
#include "ServerLauncher.h"

void RunClientManager()
{  
    int serverSocket = LaunchServer();
    AcceptMultiClient(serverSocket);
}