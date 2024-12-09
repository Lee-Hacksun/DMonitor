#ifndef COLOR_H
#define COLOR_H

// 컬러 센서 데이터 구조체
typedef struct _ColorData
{
    int red;
    int green;
    int blue;
} ColorData;

// 컬러 센서 초기화
int InitColorSensor(void);

// 컬러 센서 읽기
int ReadColorSensor(ColorData* data);

#endif
