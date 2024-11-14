#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "ClientManager.h"
#include "GUIManager.h"

int main()
{
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        // TODO : Fork에 대한 error handler 추가 
    }
    else if (pid == 0)
    {
        RunClientManager();
    }
    else
    {
        RunGUIManager();
    }

    exit(0);
}