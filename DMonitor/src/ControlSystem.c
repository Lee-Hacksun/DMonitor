#include "ControlSystem.h"
#include "DebugUtil.h"

#if _TEST
    // TODO : 테스트를 위해 필요한 헤더 추가
#else 
    // TODO : 실제 클라이언트에 필요한 헤더 추가  
#endif

int GetTemperature()
{
    #if _TEST
    // TODO : 테스트를 위한 임의의 값 반환
    return 0;

    #else 
    // TODO : 실제 센서값 반환
    return 0;

    #endif
}

void ControlBuzzer()
{
    #if _TEST
    // TODO : 테스트를 위한 코드 작성

    #else
    // TODO : 실제 동작을 위한 코드 작성

    #endif
}

void ControlLED()
{
    #if _TEST
    // TODO : 테스트를 위한 코드 작성

    #else
    // TODO : 실제 동작을 위한 코드 작성

    #endif
}