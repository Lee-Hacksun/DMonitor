cmake_minimum_required(VERSION 3.10)
project(DMonitor VERSION 1.0)

set(CMAKE_BUILD_TYPE STREQUAL "Test" CACHE STRING "Test build type")

include_directories(../utils)

set(SOURCES 
    DMonitor.c 
    GUIManager.c 
    ClientManager.c
    TabPanel.c 
    ServerLauncher.c
    DMonitorThread.c
    Tree.c
    Color.c
    FileManager.c
    ../utils/RWLock.c
    ../utils/csv.c
    ../utils/cjson/cJSON.c
    ../utils/map.c
    ../utils/logger.c
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)
endif()

add_executable(DMonitor ${SOURCES})


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(DMonitor PRIVATE DEBUG)
    target_compile_definitions(DMonitor PRIVATE TEST)
    include_directories(../testData)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    target_compile_definitions(DMonitor PRIVATE TEST)
    include_directories(../testData)
endif()

find_package(Curses REQUIRED)

target_link_libraries(DMonitor PRIVATE ${CURSES_LIBRARIES})
target_link_libraries(DMonitor PRIVATE m)

include_directories(${CURSES_INCLUDE_DIRS})