#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "DebugUtil.h"
#include "FileManager.h"

void SetEXEPath()
{
    ssize_t len = readlink("/proc/self/exe", EXEPath, sizeof(EXEPath));

    if (len != -1)
    {
        EXEPath[len] = '\0';
        strcpy(EXEPath,dirname(EXEPath));
        //printf("Setup EXEPath : %s\n", EXEPath);
    }
    else
    {
        perror("readlink");
    }
}

char* GetLogDirPath()
{
    char* logPath = malloc(sizeof(char) * PATH_MAX);

    ASSERT(strlen(EXEPath) + strlen(LOG_DIR_PATH) < PATH_MAX, "경로의 길이가 너무 깁니다.\n");
    strcpy(logPath, EXEPath);
    strcat(logPath, LOG_DIR_PATH);

    return logPath;
}