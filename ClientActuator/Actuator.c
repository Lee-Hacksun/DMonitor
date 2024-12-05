#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Actuator.h"

#if TEST
#else
int InitActuator(void)
{
    if (InitBuzzer() == -1)
    {
        printf("Failed to initialize buzzer\n");
        return -1;
    }

    if (InitLED() == -1)
    {
        printf("Failed to initialize LED\n");
        return -1;
    }

    return 0;
}
#endif

void RunActuator(int buzzer, int red, int green, int blue)
{
#if TEST
    if (buzzer == 1)
        printf("Buzzer on\n");
    else
        printf("Buzzer off\n");

    if (red == 1)
        printf("Red LED on\n");
    else
        printf("Red LED off\n");

    if (green == 1)
        printf("Green LED on\n");
    else
        printf("Green LED off\n");

    if (blue == 1)
        printf("Blue LED on\n");
    else
        printf("Blue LED off\n");
#else
    InitActuator();

    ControlBuzzer(buzzer);
    ControlRedLED(red);
    ControlGreenLED(green);
    ControlBlueLED(blue);
#endif
}