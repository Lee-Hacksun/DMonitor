#ifndef LED_ACTUATOR_H
#define LED_ACTUATOR_H

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LEDSTATE;

// LED 초기화
int InitLED(void);

// 특정 LED 제어
int ControlLED(int color, LEDSTATE state);

// RGB LED 색상 설정
int SetColor(LEDSTATE red, LEDSTATE green, LEDSTATE blue);

// 모든 LED 끄기
int TurnOffAllLED(void);

#endif
