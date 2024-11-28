#ifndef BUZZER_ACTUATOR_H
#define BUZZER_ACTUATOR_H

typedef enum {
    BUZZER_OFF = 0,
    BUZZER_ON = 1
} BUZZERSTATE;

// 부저 초기화
int InitBuzzer(void);

// 부저 제어
int ControlBuzzer(BUZZERSTATE state);

#endif
