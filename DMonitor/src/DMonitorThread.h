#ifndef DMONITORTHREAD_H
#define DMONITORTHREAD_H

#include "csv.h"
#include "SensorData.h"

#define MAX_CONNECTION 128
#define MAX_SENSING_COUNT 17280 // Maximum number of times sensing every 5 seconds

#define IS_SUPER_DO 1<<7
#define EMERGENCY 1<<6
#define IS_CHANGED 1<<5
#define NONE2 1<<4
#define BUZZER 1<<3
#define LED_RED 1<<2
#define LED_GREEN 1<<1
#define LED_BLUE 1<<0

#define CHEERY_BLOSSOM "CherryBlossom"
#define MAPLE "Maple"

#define SPECIES_CHERRY_BLOSSOM 0
#define SPECIES_MAPLE 1

typedef struct _TotalData
{
    int count;
    SensorData* sensorData[MAX_SENSING_COUNT];
} TotalData;

TotalData* InitTotalData();
int GetColorDistance(Color targetColor, Color nowCOlor);
void UpdateProgress(char* clientID, int progress);
void ProcessDataLog(char* clientID, TotalData* totalData);

void SetupControlCode(unsigned char code);

char* RegistClient(int clientSocket);

SensorData* ReadSensorData(int clientSocket);
Color GetRegisteredColor(char* clientID);

void* DMonitorThreadAction(void* arg);
void DMonitorThreadCreate(int clientSocket);

#endif