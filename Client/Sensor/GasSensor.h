#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

// 가스센서 초기화 함수
int InitGasSensor(void);

// 가스 센서 값 읽는 함수
int ReadGasSensor(char channel);

#endif