#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#define DISPLAY_OVERVIEW 1
#define DISPLAY_DETAILS 2

void InitCurses();

int DisplayOverview();
int DisplayDetails();
void RunGUIManager();

#endif