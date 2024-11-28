#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Color.h"
#include "RWLock.h"

#define BUFFER_SIZE 1024

extern RWLock g_rwlock;
extern Color g_color;
extern char g_colorClientID[BUFFER_SIZE];

void RunClientManager(int inputPipe);

#endif