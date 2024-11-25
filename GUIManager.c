#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cjson/cJSON.h"

#include "Color.h"
#include "DebugUtil.h"
#include "GUIManager.h"
#include "TabPanel.h"
#include "Tree.h"

char* help[HELP_MESSAGE_SIZE] =
{
    "[[OverView]]",
    "UP arrow | DOWN arrow : move tab",
    "LEFT arrow | RIGHT arrow : move 5 tab",
    "PAGE up | PAGE down : move tab page",
    "H : display manual",
    "Q : return to Overview",
    "",
    "[[DtailView]]",
    "C : color assignment"
};

    char* testData[] = 
    {
        "1123",
        "2123",
        "3123",
        "4123",
        "5123",
        "6123",
        "7123",
        "8123",
        "9123",
        "10123",
        "11123",
        "12123",
        "13123",
        "14123",
        "15123"
    };

void SendColor(int fd, char* clientID, Color color)
{
    cJSON* json = cJSON_CreateObject();
    if (json == NULL)
    {
        ASSERT(0, "Failed to create json");
        return;
    }

    cJSON_AddStringToObject(json, "clientID", clientID);

    cJSON* jsonColor = cJSON_CreateObject();
    cJSON_AddNumberToObject(jsonColor, "Red", color.Red);
    cJSON_AddNumberToObject(jsonColor, "Green", color.Green);
    cJSON_AddNumberToObject(jsonColor, "Blue", color.Blue);
    cJSON_AddItemToObject(json, "color", jsonColor);

    char* jsonString = cJSON_Print(json);
    if (jsonString == NULL)
    {
        cJSON_Delete(json);
        ASSERT(0, "Failed to create json");
        return;
    }

    write(fd, jsonString, strlen(jsonString));

    free(jsonString);
    cJSON_Delete(json);
}

void InitCurses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);

    if (has_colors())
    {
        start_color();

        init_color(COLOR_LEAF, 200, 800, 100);
        init_color(COLOR_WOOD, 757, 604, 420);
        init_color(COLOR_CHERRYBLOSSOM, 1000, 715, 757);
        init_pair(COLOR_LEAF_BLACK, COLOR_LEAF, COLOR_BLACK);
        init_pair(COLOR_WOOD_BLACK, COLOR_WOOD, COLOR_BLACK);
        init_pair(COLOR_CHERRYBLOSSOM_BLACK, COLOR_CHERRYBLOSSOM, COLOR_BLACK);
    }

    clear();
    refresh();
}

void DrawBorder(WINDOW* window)
{
    box(window, 0, 0);
}

void DrawTitle()
{
    ASSERT(stdscr != NULL, "curses must be init");

    attron(COLOR_PAIR(COLOR_LEAF_BLACK));
    mvwprintw(stdscr, 0, (COLS - strlen(PROGRAM_TITLE)) /2, "%s", PROGRAM_TITLE);
    attroff(COLOR_PAIR(COLOR_LEAF_BLACK));
}

void ClearScreen()
{
    wclear(stdscr);
    DrawTitle(PROGRAM_TITLE);
    wrefresh(stdscr);
}

int HandleKeyboardInput(Panel* panel, int* selectedTab)
{
    int ch = getch();    
    switch(ch)
    {
        case KEY_DOWN:
            if (panel != NULL)
            {
                IncreasePanelSelect(panel);
            }
            break;

        case KEY_UP:
            if (panel != NULL)
            {
                DecreasePanelSelect(panel);
            }
            break;

        case KEY_RIGHT:
            for (int i = 0; i < 5; i++)
            {
                if (panel != NULL)
                {
                    IncreasePanelSelect(panel);
                }
            }
            break;

        case KEY_LEFT:
            for (int i = 0; i < 5; i++)
            {
                if (panel != NULL)
                {
                    DecreasePanelSelect(panel);
                }
            }
            break;        

        case KEY_NPAGE:
            if (panel != NULL)
            {
                for (int i = 0; i < panel->height; i++)
                {
                    IncreasePanelOffset(panel);
                }
            }
            break;

        case KEY_PPAGE:
            if (panel != NULL)
            {
                for (int i = 0; i < panel->height; i++)
                {
                    DecreasePanelOffset(panel);
                }
            }
            break;

        case 'Q':
        case 'q':
            if (selectedTab != NULL)
            {
                *selectedTab = -1;
            }
            ClearScreen();
            return DISPLAY_OVERVIEW;

        case 'H':
        case 'h':
            ClearScreen();
            return DISPLAY_HELP;

        case '\n':
            if( selectedTab != NULL)
            {
                *selectedTab = panel->offset + panel->selected;
                ClearScreen();
                return DISPLAY_DETAILS;
            }
            break;

        case 'C':
        case 'c':
            if (selectedTab != NULL && *selectedTab == -1)
            {
                ClearScreen();
                return DISPLAY_COLOR_SETTING;
            }
            break;

        default:
            return DISPLAY_CURRENT;
    }
    return DISPLAY_CURRENT;
}

unsigned char GetColorElement(WINDOW* window, int Yposition, int Xposition)
{   
    int input = -1;
    while (1)
    {
        mvwscanw(window, Yposition, Xposition, "%d", &input);
        if (input >= 0 && 255 >= input)
        {
            return (unsigned char)input;
        }
        wmove(window, Yposition, Xposition);
        wclrtoeol(window);
        DrawBorder(window);
        wrefresh(window);
    }
}

int DisplayOverview(WINDOW* window, Panel* panel, int* selectedTab)
{
    DrawPanel(window, panel, LEFT);
    return HandleKeyboardInput(panel, selectedTab);
}

int DisplayDetails(WINDOW* windowLeft, WINDOW* windowRight, Panel* panel, int* selectedTab)
{
    DrawPanel(windowLeft, panel, LEFT);
    DrawTree(windowRight, GetTree(windowRight), COLOR_CHERRYBLOSSOM, COLOR_WOOD_BLACK);

    *selectedTab = -1;
    return HandleKeyboardInput(panel, selectedTab);
}

int DisplayHelp(WINDOW* window, Panel* panel)
{  
    DrawPanel(window, panel, CENTER);    
    return HandleKeyboardInput(panel, NULL);
}

void DisplayColorSetting(WINDOW* window, char* clientID, int fd)
{
    int input = -1;
    Color color;
    DrawBorder(window);

    mvwprintw(window, 1, 1, "Red: ");
    mvwprintw(window, 2, 1, "Green: ");
    mvwprintw(window, 3, 1, "Blue: ");
    wrefresh(window);

    echo();
    curs_set(2);

    color.Red = GetColorElement(window, 1, 6);
    color.Green = GetColorElement(window, 2, 8);
    color.Blue = GetColorElement(window, 3, 7);

    noecho();
    curs_set(0);

    SendColor(fd, clientID, color);

    wclear(window);
    ClearScreen();
}

void RunGUIManager(int inputPipe)
{
    InitCurses();
    DrawTitle();

    WINDOW* fullScreenWindow = newwin(LINES - MARGIN_TOP * 2, COLS - MARGIN_LEFT * 2, MARGIN_TOP, MARGIN_LEFT);
    WINDOW* cropScreenWindow = newwin(LINES / 2, COLS / 2, LINES / 4, COLS / 4);
    WINDOW* leftLopsidedWindow = newwin(LINES - MARGIN_TOP * 2, (COLS / 3) * 2 - MARGIN_LEFT * 2, MARGIN_TOP, MARGIN_LEFT);
    WINDOW* rightLopsidedWindow = newwin(LINES - MARGIN_TOP * 2, ((COLS) / 3) * 1 - MARGIN_LEFT * 2, MARGIN_TOP, (COLS / 3) * 2 + MARGIN_LEFT);

    PanelMessage* overviewMessage = NewPanelMessage(testData, 15);
    PanelMessage* detailMessage = NewPanelMessage(testData, 15); 
    PanelMessage* helpMessage = NewPanelMessage(help, HELP_MESSAGE_SIZE);

    Panel* overviewPanel = wNewPanel(fullScreenWindow, MARGIN_TOP, MARGIN_LEFT, overviewMessage);
    Panel* detailPanel = wNewPanel(leftLopsidedWindow, MARGIN_TOP, MARGIN_LEFT, detailMessage);
    Panel* helpPanel = wNewPanel(fullScreenWindow, MARGIN_TOP, MARGIN_LEFT, helpMessage);

    char* clientID;
    int selectedTab = 0;
    int displayStatus = DISPLAY_OVERVIEW;
    int displayNext;

    while(1)
    {
        switch (displayStatus)
        {
        case DISPLAY_OVERVIEW:
            if ((displayNext = DisplayOverview(fullScreenWindow, overviewPanel, &selectedTab)) != DISPLAY_CURRENT)
            {
                displayStatus = displayNext;
            } 
            break;

        case DISPLAY_DETAILS:
            if ((displayNext = DisplayDetails(leftLopsidedWindow, rightLopsidedWindow, detailPanel, &selectedTab)) != DISPLAY_CURRENT)
            {
                clientID = detailPanel->panelMessage->informations[overviewPanel->selected + overviewPanel->offset];
                displayStatus = displayNext;
            } 
            break;

        case DISPLAY_HELP:
            if ((displayNext = DisplayHelp(fullScreenWindow, helpPanel)) != DISPLAY_CURRENT)
            {
                displayStatus = displayNext;
            } 
            break;

        case DISPLAY_COLOR_SETTING:
            DisplayColorSetting(cropScreenWindow, clientID, inputPipe);
            displayStatus = DISPLAY_DETAILS;
            break;
            
        default:
            ASSERT(displayStatus && FALSE, "undefined displayStatus");
            displayStatus = DISPLAY_OVERVIEW;
            break;
        }
    }
}