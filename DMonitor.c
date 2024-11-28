#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "ClientManager.h"
#include "GUIManager.h"

#include "csv.h"
#include "DebugUtil.h"

int main()
{
    #if _TEST

    ParseCSV("../test.csv");

    #elif !_TEST
    int inputPipe[2];

    if ( pipe(inputPipe) == -1)
    {
        perror("Failed to create pipe.");
        exit(1);
    }

    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Failed to fork process");
    }
    else if (pid == 0)
    {   
        close(inputPipe[1]);
        RunClientManager(inputPipe[0]);
    }
    else
    {
        close(inputPipe[0]);
        RunGUIManager(inputPipe[1]);
    }

    exit(0);

    #endif
}