#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "logger.h"

pthread_mutex_t g_log_file;
char g_log_path[PATH_MAX];

FILE* GetLogStream()
{
    if (strcmp(g_log_path, "") == 0)
    {
        return NULL;
    }

    FILE* stream = fopen(g_log_path, "a+");
    if (stream == NULL)
    {
        perror("fopen");
        return NULL;
    }

    return stream;
}


void LogPrintf(const char* format, ...) 
{
    pthread_mutex_lock(&g_log_file);
    FILE* stream = GetLogStream();

    time_t now;
    time(&now);
    struct tm* local = localtime(&now);

    
    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", local);
    
    fprintf(stream, "[%s] ", timeString);

    
    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fclose(stream);
    pthread_mutex_unlock(&g_log_file);
}

void InitLogger(char* path)
{    
    pthread_mutex_init(&g_log_file, NULL);

    strcpy(g_log_path, path);
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        file = (fopen(path, "w"));
        if (file == NULL)
        {
            perror("fopen");
        }
    }

    fclose(file);
    LogPrintf("%s\n", "로그를 시작합니다.");
}