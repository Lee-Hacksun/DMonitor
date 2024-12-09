cmake_minimum_required(VERSION 3.10)
project(DMonitor VERSION 1.0)

set(CMAKE_BUILD_TYPE STREQUAL "Test" CACHE STRING "Test build type")

if (CMAKE_BUILD_TYPE STREQUAL "Test")
    set(SOURCES 
        cjson/cJSON.c
        Client.c
        Actuator.c
    )
else()
    set(SOURCES 
        cjson/cJSON.c
        Client.c
        Actuator.c
        Actuator/BuzzerActuator.c
        Actuator/LedActuator.c
    )
endif()

add_executable(DMonitor ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(DMonitor PRIVATE DEBUG)
    target_compile_definitions(DMonitor PRIVATE TEST)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    target_compile_definitions(DMonitor PRIVATE TEST)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Test")
    target_link_libraries(DMonitor PRIVATE ${CURSES_LIBRARIES} wiringPi)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CURSES_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/Actuator
)