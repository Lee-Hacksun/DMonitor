#ifndef SENSORDATA_H
#define SENSORDATA_H

#include "Color.h"

typedef struct _SensorData
{
    int gas;
    int flame;
    float humidity;
    float temp;
    int light;
    Color color;
} SensorData;

#endif