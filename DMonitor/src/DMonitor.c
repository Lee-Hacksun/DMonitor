#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "ClientManager.h"
#include "GUIManager.h"

#include "DebugUtil.h"
#include "FileManager.h"
#include "FileLock.h"

#include "csv.h"

char EXEPath[PATH_MAX];

int main()
{   
    int inputPipe[2];
    SetEXEPath();

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

    pthread_mutex_destroy(&g_client_list_lock);
    pthread_mutex_destroy(&g_sensor_lock);
    pthread_mutex_destroy(&g_progress_lock);

    exit(0);
}