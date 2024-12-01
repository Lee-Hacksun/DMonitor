#ifndef DMONITORTHREAD_H
#define DMONITORTHREAD_H

#define MAX_CONNECTION 128
#define MAX_SENSING_COUNT 17280 // Maximum number of times sensing every 5 seconds

#include "SensorData.h"

typedef struct _TotalData
{
    int count;
    SensorData* sensorData[MAX_SENSING_COUNT];
} TotalData;

TotalData* InitTotalData();
void ProcessDataLog(char* clientID, TotalData* totalData);

char* RegistClient(int clientSocket);
SensorData* ReadSensorData(int clientSocket);
void* DMonitorThreadAction(void* arg);
void InitDMonitorThread();
void DMonitorThreadCreate(int clientSocket);

#endif