cmake_minimum_required(VERSION 3.10)
project(DMonitor VERSION 1.0)

set(CMAKE_BUILD_TYPE STREQUAL "Test" CACHE STRING "Test build type")

set(SOURCES 
    DMonitor.c 
    GUIManager.c 
    ClientManager.c
    TabPanel.c 
    ServerLauncher.c
    DMonitorThread.c
    Tree.c
    cjson/cJSON.c
)

add_executable(DMonitor ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(DMonitor PRIVATE DEBUG)
    target_compile_definitions(DMonitor PRIVATE TEST)
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    target_compile_definitions(DMonitor PRIVATE TEST)
endif()

find_package(Curses REQUIRED)

target_link_libraries(DMonitor PRIVATE ${CURSES_LIBRARIES})

include_directories(${CURSES_INCLUDE_DIRS})