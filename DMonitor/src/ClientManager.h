#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <limits.h>
#include <stdio.h>

#include "Color.h"
#include "RWLock.h"

#define BUFFER_SIZE 1024
#define CLIENT_ID_SIZE 128


extern RWLock g_color_rwlock;
extern Color g_color;
extern char g_colorClientID[BUFFER_SIZE];

extern RWLock g_control_code_rwlock;
extern unsigned char g_control_code;

typedef struct _ClientINFO
{
    char clientName[CLIENT_ID_SIZE];
} ClientINFO;

void RunClientManager(int inputPipe);
void CheckWorkingDirectory();
void CheckSensorFile();
void CheckClientListFile();

#endif