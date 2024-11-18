#include <stdio.h>
#include <time.h>
#include <string.h>
#include "SensorLog.h"

void WriteLog(const char *client_id, float temp, float humi, const char *color,
              int light, int flame, int gas)
{
    FILE *fp;
    time_t current_time;
    struct tm *time_info;
    char timestamp[20];

    // 현재 시간 가져오기
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    // 파일 존재 여부 확인
    fp = fopen("SensorLog.csv", "r");
    if (fp == NULL)
    {
        // 파일이 없으면 새로 만들고 해더 추가
        fp = fopen("SensorLog.csv", "w");
        if (fp != NULL)
        {
            fprintf(fp, "ClientID,Time,Temp,Humid,Color,Light,Flame,Gas\n");
            fclose(fp);
        }
    }
    else
        fclose(fp);

    // 로그 추가
    fp = fopen("SensorLog.csv", "a");
    if (fp != NULL)
    {
        fprintf(fp, "%s,%s,%f,%f,%s,%d,%d,%d\n",
                client_id, timestamp, temp, humi, color, light, flame, gas);
        fclose(fp);
    }
}