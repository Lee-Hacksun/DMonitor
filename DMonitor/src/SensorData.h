#ifndef SENSORDATA_H
#define SENSORDATA_H

#include "Color.h"

typedef struct _SensorData
{
    int gas;
    int flame;
    int humidity;
    int temp;
    int light;
    Color color;
} SensorData;

#endif