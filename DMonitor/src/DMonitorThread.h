#ifndef DMONITORTHREAD_H
#define DMONITORTHREAD_H

#define MAX_CONNECTION 128

void* DMonitorThreadAction(void* arg);
void InitDMonitorThread();
void DMonitorThreadCreate(int clientSocket);

#endif