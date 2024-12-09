#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <ncurses.h>

#include "Color.h"
#include "TabPanel.h"

#define PROGRAM_TITLE "D-MONITOR"

#define HELP_MESSAGE_SIZE 11
#define TITLE_MESSAGE_SIZE 8

#define DISPLAY_CURRENT -1
#define DISPLAY_OVERVIEW 1
#define DISPLAY_DETAILS 2
#define DISPLAY_HELP 3
#define DISPLAY_COLOR_SETTING 4
#define DISPLAY_TURNOFF_BUZZER 5
#define DISPLAY_REFRESh_OVERVEIW 6
#define CHANGE_CLIENTINFOS_ORDER 7

#define COLOR_LEAF 1
#define COLOR_WOOD 2
#define COLOR_CHERRYBLOSSOM 3
#define COLOR_CHERRYBLOSSOM_1 4
#define COLOR_CHERRYBLOSSOM_2 5
#define COLOR_CHERRYBLOSSOM_3 6
#define COLOR_MAPLE_1 7
#define COLOR_MAPLE_2 8
#define COLOR_MAPLE_3 9

#define COLOR_LEAF_BLACK 101
#define COLOR_WOOD_BLACK 102
#define COLOR_CHERRYBLOSSOM_BLACK 103
#define COLOR_CHERRYBLOSSOM_1_BLACK 104
#define COLOR_CHERRYBLOSSOM_2_BLACK 105
#define COLOR_CHERRYBLOSSOM_3_BLACK 106
#define COLOR_MAPLE_1_BLACK 107
#define COLOR_MAPLE_2_BLACK 108
#define COLOR_MAPLE_3_BLACK 109

#define MAX_CLIENT_ID_SIZE 128
#define MAX_REGION_CODE_SIZE 4
#define MAX_SPECIES_SIZE 128

#define ASCENDING 1
#define DESCENDING 0

typedef struct _ClientInfos
{
    char clientID[MAX_CLIENT_ID_SIZE];
    char regionCode[MAX_REGION_CODE_SIZE];
    char species[MAX_SPECIES_SIZE];
    int progress; 
    int size;
} ClientInfos;

typedef struct _ClientDetailInfos
{
    int gas;
    int flame;
    int light;
    float temp;
    float humidity;
    unsigned char colorRed;
    unsigned char colorGreen;
    unsigned char colorBlue;
    int progress;
} ClientDetailInfos;

void Merge(ClientInfos* array, int left, int mid, int right, int ascending);
void MergeSort(ClientInfos* array, int left, int right, int ascending);
void SortClientInfos(ClientInfos* infos, int ascending);

ClientInfos* GetClientInfos(ClientInfos* clientInfos, int* clientInfosSize);
char** ClientInfosToString(ClientInfos* clientInfos, int size);

int GetTreeColor(char* specise, int progress);

void GetDetailPanel(WINDOW* win, char* clientID);
void DestroyDetailPanel();

void SendColor(int fd, char* clientID, Color color);
void TurnOffBuzzer(int fd);

void InitCurses();
void ClearScreen();
void DrawBorder(WINDOW* window);

void DrawTitle();
int HandleKeyboardInput(Panel* panel, int* selectedTab);
unsigned char GetColorElement(WINDOW* window, int Yposition, int Xposition);
int DisplayOverview(WINDOW* window, Panel* panel, int* selectedTab);
int DisplayDetails(WINDOW* windowLeft, WINDOW* windowRight, char* clientID, int colorCode, int* selectedTab);
int DisplayHelp(WINDOW* window, Panel* panel);
int DisplayColorSetting(WINDOW* window, char* clientID, int fd);
void RunGUIManager(int inputPipe);

#endif