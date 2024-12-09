#ifndef LED_ACTUATOR_H
#define LED_ACTUATOR_H

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} LEDSTATE;

// LED 초기화
int InitLED(void);

void ControlRedLED(int state);

void ControlGreenLED(int state);

void ControlBlueLED(int state);

void TurnOffAllLED(void);

#endif
