#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <pthread.h>

#define PATH_MAX 4096

extern pthread_mutex_t g_log_file;
extern char g_log_path[PATH_MAX];

FILE* GetLogStream();
void LogPrintf(const char* format, ...);
void InitLogger(char* path);
#endif