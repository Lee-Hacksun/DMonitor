#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <ncurses.h>

#include "Color.h"
#include "TabPanel.h"

#define PROGRAM_TITLE "D-MONITOR"

#define HELP_MESSAGE_SIZE 9

#define DISPLAY_CURRENT -1
#define DISPLAY_OVERVIEW 1
#define DISPLAY_DETAILS 2
#define DISPLAY_HELP 3
#define DISPLAY_COLOR_SETTING 4

#define COLOR_LEAF 1
#define COLOR_WOOD 2
#define COLOR_CHERRYBLOSSOM 3

#define COLOR_LEAF_BLACK 1
#define COLOR_WOOD_BLACK 2
#define COLOR_CHERRYBLOSSOM_BLACK 3

void SendColor(int fd, char* clientID, Color color);

void InitCurses();
void ClearScreen();
void DrawBorder(WINDOW* window);

void DrawTitle();
int HandleKeyboardInput(Panel* panel, int* selectedTab);
unsigned char GetColorElement(WINDOW* window, int Yposition, int Xposition);
int DisplayOverview(WINDOW* window, Panel* panel, int* selectedTab);
int DisplayDetails(WINDOW* windowLeft, WINDOW* windowRight, Panel* panel, int* selectedTab);
int DisplayHelp(WINDOW* window, Panel* panel);
void DisplayColorSetting(WINDOW* window, char* clientID, int fd);
void RunGUIManager(int inputPipe);

#endif