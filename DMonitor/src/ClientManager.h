#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <limits.h>

#include "Color.h"
#include "RWLock.h"

#define BUFFER_SIZE 1024
#define CLIENT_NAME_SIZE 128
#define PATH_MAX 4096

#define CLIENT_LIST_PATH "/log/ClientList.csv"
#define LOG_PATH "/log"
//#define CLIENT_LOG_PATH "log/CLIENT_LOG/"

#define CLIENT_LIST_CSV_CLIENT_ID 0

extern RWLock g_color_rwlock;
extern Color g_color;
extern char g_colorClientID[BUFFER_SIZE];
extern char EXEPath[PATH_MAX];

typedef struct _ClientINFO
{
    char clientName[CLIENT_NAME_SIZE];
} ClientINFO;

void RunClientManager(int inputPipe);
void SetEXEPath();
void CheckWorkingDirectory();
void CheckClientListFile();

#endif