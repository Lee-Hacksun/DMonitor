#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#define PATH_MAX 4096

#define LOG_DIR_PATH "/log"
#define CLIENT_LIST_PATH "/ClientList.csv"
#define PROGRESS_LIST_PATH "/Progress.csv"
#define SENSOR_PATH "/Sensor.csv"

#define CLIENT_LIST_CSV_HEADER "clientID,Species,Red,Green,Blue,RegionCode\n"
#define CLIENT_LIST_CSV_CLIENT_ID 0
#define CLIENT_LIST_CSV_SPECIES 1
#define CLIENT_LIST_CSV_COLOR_RED 2
#define CLIENT_LIST_CSV_COLOR_GREEN 3
#define CLIENT_LIST_CSV_COLOR_BLUE 4
#define CLIENT_LIST_CSV_REGION_CODE 5

#define PROGRESS_LIST_CSV_HEADER "ClientID,Progress\n"
#define PROGRESS_LIST_CSV_CLIENT_ID 0
#define PROGRESS_LIST_CSV_PROGRESS 1

#define SENSOR_CSV_HEADER "ClientID,flame,gas,humidity,light,temp,Red,Green,Blue,Progress\n"
#define SENSOR_CSV_CLIENT_ID 0
#define SENSOR_CSV_GAS 1
#define SENSOR_CSV_FLAME 2
#define SENSOR_CSV_HUMIDITY 3
#define SENSOR_CSV_LIGHT 4
#define SENSOR_CSV_TEMP 5
#define SENSOR_CSV_RED 6
#define SENSOR_CSV_GREEN 7
#define SENSOR_CSV_BLUE 8 
#define SENSOR_CSV_PROGRESS 9

extern char EXEPath[PATH_MAX];

void SetEXEPath();
char* GetLogDirPath();

#endif