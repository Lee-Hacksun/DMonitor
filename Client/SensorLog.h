#ifndef SENSORLOG_H
#define SENSORLOG_H

// 로그 기록 함수
void WriteLog(const char *client_id, float temp, float humi, const char *color,
              int light, int flame, int gas);

#endif