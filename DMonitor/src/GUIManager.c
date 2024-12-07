#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cjson/cJSON.h"

#include "Color.h"
#include "csv.h"
#include "DebugUtil.h"
#include "FileManager.h"
#include "FileLock.h"
#include "GUIManager.h"
#include "TabPanel.h"
#include "Tree.h"

int clientDetailInfosSize = 0;
ClientDetailInfos* clientDetailInfos = NULL;
char** clientDetailInfosString = NULL;
PanelMessage* clientDetailPanelMessage = NULL;
Panel* clientDetailPanel = NULL;


char* help[HELP_MESSAGE_SIZE] =
{
    "[[OverView, HelpPage]]",
    "UP arrow | DOWN arrow : move tab",
    "LEFT arrow | RIGHT arrow : move 5 tab",
    "PAGE up | PAGE down : move tab page",
    "H : display manual",
    "Q : return to Overview",
    "B : Turn off buzzer",
    "",
    "[[DtailView]]",
    "C : color assignment"
};

char* title[TITLE_MESSAGE_SIZE] = {
    " ____        __  __             _ _             ",
    "|  _ \\      |  \\/  | ___  _ __ (_) |_ ___  _ __ ",
    "| | | |_____| |\\/| |/ _ \\| '_ \\| | __/ _ \\| '__|",
    "| |_| |_____| |  | | (_) | | | | | || (_) | |   ",
    "|____/      |_|  |_|\\___/|_| |_|_|\\__\\___/|_|   ",
    "",
    "",
    "If the actuator client is connected, press any key to continue"
};

void Merge(ClientInfos* array, int left, int mid, int right, int ascending) 
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    ClientInfos* leftArr = malloc(n1 * sizeof(ClientInfos));
    ClientInfos* rightArr = malloc(n2 * sizeof(ClientInfos));

    for (i = 0; i < n1; i++) leftArr[i] = array[left + i];
    for (j = 0; j < n2; j++) rightArr[j] = array[mid + 1 + j];

    i = 0; j = 0; k = left;

    while (i < n1 && j < n2) {
        if ((ascending && leftArr[i].progress <= rightArr[j].progress) ||
            (!ascending && leftArr[i].progress > rightArr[j].progress)) {
            array[k++] = leftArr[i++];
        } else {
            array[k++] = rightArr[j++];
        }
    }

    while (i < n1) array[k++] = leftArr[i++];
    while (j < n2) array[k++] = rightArr[j++];

    free(leftArr);
    free(rightArr);
}

void MergeSort(ClientInfos* array, int left, int right, int ascending) 
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        MergeSort(array, left, mid, ascending);
        MergeSort(array, mid + 1, right, ascending);
        Merge(array, left, mid, right, ascending);
    }
}

void SortClientInfos(ClientInfos* infos, int ascending) 
{
    if (infos == NULL || infos->size <= 1) return;
    MergeSort(infos, 0, infos->size - 1, ascending);
}

ClientInfos* GetClientInfos(ClientInfos* clientInfos, int* clientInfosSize)
{
    if (clientInfos != NULL)
    {
        free(clientInfos);
    }

    char* path = GetLogDirPath();
    strcat(path, CLIENT_LIST_PATH);

    FILE* csvFile = fopen(path, "r");
    if (csvFile == NULL)
    {
        perror("fopen");
        return NULL;
    }

    int index = CLIENT_LIST_CSV_CLIENT_ID;
    CSV* csv = ParseCSVOption(csvFile, Distinct, &index, NULL, NULL, NULL);
    free(path);
    fclose(csvFile);

    clientInfos = malloc(sizeof(ClientInfos) * csv->csvSize);
    for (int i = 0; i < csv->csvSize; i++)
    {
        strcpy(clientInfos[i].clientID, csv->data[i][CLIENT_LIST_CSV_CLIENT_ID]);
        strcpy(clientInfos[i].species, csv->data[i][CLIENT_LIST_CSV_SPECIES]);
        strcpy(clientInfos[i].regionCode, csv->data[i][CLIENT_LIST_CSV_REGION_CODE]);
    }
    *clientInfosSize = csv->csvSize;
    DestroyCSV(csv);

    path = GetLogDirPath();
    strcat(path, PROGRESS_LIST_PATH);

    csvFile = fopen(path, "r");
    if (csvFile == NULL)
    {
        perror("fopen");
        return NULL;
    }

    index = PROGRESS_LIST_CSV_CLIENT_ID;
    csv = ParseCSVOption(csvFile, Distinct, &index, NULL, NULL, NULL);
    free(path);
    fclose(csvFile); 

    for (int i = 0; i < csv->csvSize; i++)
    {
        clientInfos[i].progress = atoi(csv->data[i][PROGRESS_LIST_CSV_PROGRESS]);
    }
    clientInfos->size = csv->csvSize;
    SortClientInfos(clientInfos, DESCENDING);
    DestroyCSV(csv);

    return clientInfos;
}

char** ClientInfosToString(ClientInfos* clientInfos, int size)
{
    char** clientinfosString = malloc(sizeof(char*) * size + 1);
    clientinfosString[0] = malloc(sizeof(char) * COLS - MARGIN_LEFT * 2);
    sprintf(clientinfosString[0], "%-20s %-20s %-20s %-20s\n", "ClientID", "Species", "RegionCode", "Progress");

    for(int i = 1; i < size + 1; i++)
    {
        clientinfosString[i] = malloc(sizeof(char) * COLS - MARGIN_LEFT * 2);
        sprintf(clientinfosString[i], "%-20s %-20s %-20s %d%%\n", clientInfos[i - 1].clientID, clientInfos[i - 1].species, clientInfos[i - 1].regionCode, clientInfos[i - 1].progress);
    }

    return clientinfosString;
}

int GetTreeColor(char* specise, int progress)
{
    int colors[2][3] = 
    {
        {COLOR_MAPLE_1_BLACK, COLOR_MAPLE_2_BLACK, COLOR_MAPLE_3_BLACK},
        {COLOR_CHERRYBLOSSOM_1_BLACK, COLOR_CHERRYBLOSSOM_2_BLACK, COLOR_CHERRYBLOSSOM_3_BLACK}
    };

    int speciseSelect = -1;
    int progressSelect = -1;

    if (strcmp(specise, "Maple") == 0)
    {
        speciseSelect = 0;
    }
    else if (strcmp(specise, "CherryBlossom") == 0)
    {
        speciseSelect = 1;
    }

    if (progress >= 0 && progress < 20)
    {
        progressSelect = 0;
    }
    else if (progress >= 20 && progress < 80)
    {
        progressSelect = 1;
    }
    else if (progress >= 80 && progress <= 100)
    {
        progressSelect = 2;
    }

    return colors[speciseSelect][progressSelect];
}

void GetDetailPanel(WINDOW* win, char* clientID)
{
    // 데이터 읽기
    char* path = GetLogDirPath();
    strcat(path, SENSOR_PATH);

    FILE* csvFile = fopen(path, "r");
    if (csvFile == NULL)
    {
        perror("fopen");
        return;
    }

    int index = SENSOR_CSV_CLIENT_ID;
    CSV* csv = ParseCSVOption(csvFile, Select, &index, clientID, NULL, NULL);
    free(path);
    fclose(csvFile);    

    clientDetailInfos = malloc(sizeof(ClientDetailInfos) * csv->csvSize);
    for (int i = 0; i < csv->csvSize; i++)
    {
        clientDetailInfos[i].gas = atof(csv->data[i][SENSOR_CSV_GAS]);
        clientDetailInfos[i].flame = atof(csv->data[i][SENSOR_CSV_FLAME]);
        clientDetailInfos[i].humidity = atof(csv->data[i][SENSOR_CSV_HUMIDITY]);
        clientDetailInfos[i].light = atof(csv->data[i][SENSOR_CSV_LIGHT]);
        clientDetailInfos[i].temp = atof(csv->data[i][SENSOR_CSV_TEMP]);
        clientDetailInfos[i].colorRed = atoi(csv->data[i][SENSOR_CSV_RED]);
        clientDetailInfos[i].colorGreen = atoi(csv->data[i][SENSOR_CSV_GREEN]);
        clientDetailInfos[i].colorBlue = atoi(csv->data[i][SENSOR_CSV_BLUE]);
        clientDetailInfos[i].progress = atoi(csv->data[i][SENSOR_CSV_PROGRESS]);
    }
    clientDetailInfosSize = csv->csvSize;

    DestroyCSV(csv);

    // 문자열 만들기 
    clientDetailInfosString = malloc(sizeof(char*) * clientDetailInfosSize + 1);
    clientDetailInfosString[0] = malloc(sizeof(char) * COLS - MARGIN_LEFT * 2);
    sprintf(clientDetailInfosString[0], "%-10s %-10s %-10s %-10s %-15s %-10s %-10s %-10s %-10s\n", "Flame", "Gas", "Humidity", "Light", "Temperature", "Red", "Green", "Blue", "Progress");

    for(int i = 1; i < clientDetailInfosSize + 1; i++)
    {
        clientDetailInfosString[i] = malloc(sizeof(char) * COLS - MARGIN_LEFT * 2);
        sprintf(clientDetailInfosString[i], "%-10d %-10d %-10.2f %-10d %-15.2f %-10d %-10d %-10d %d%%\n", clientDetailInfos[i - 1].flame, clientDetailInfos[i - 1].gas, clientDetailInfos[i - 1].humidity, clientDetailInfos[i - 1].light, clientDetailInfos[i - 1].temp, clientDetailInfos[i - 1].colorRed, clientDetailInfos[i - 1].colorGreen, clientDetailInfos[i - 1].colorBlue, clientDetailInfos[i - 1].progress);
    }

    clientDetailPanelMessage = NewPanelMessage(clientDetailInfosString, clientDetailInfosSize + 1);
    clientDetailPanel = wNewPanel(win, MARGIN_TOP, MARGIN_LEFT, clientDetailPanelMessage);
}

void DestroyDetailPanel()
{
    for (int i = 0; i < clientDetailInfosSize + 1; i++)
    {
        free(clientDetailInfosString[i]);
    }
    free(clientDetailInfosString);
    free(clientDetailInfos);
}

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

    ssize_t a = write(fd, jsonString, strlen(jsonString));

    cJSON_Delete(json);
}

void TurnOffBuzzer(int fd)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "BuzzerOff", 1); 

    char* jsonString = cJSON_Print(json);
    ssize_t a = write(fd, jsonString, strlen(jsonString));

    cJSON_Delete(json);

    mvwprintw(stdscr, LINES - 1, 0, "Shut down the buzzer. Press any key to continue");
    getch();
    ClearScreen();
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
        init_color(COLOR_CHERRYBLOSSOM_1, 133, 545, 133);
        init_color(COLOR_CHERRYBLOSSOM_2, 741, 690, 596);
        init_color(COLOR_CHERRYBLOSSOM_3, 1000, 752, 796);
        init_color(COLOR_MAPLE_1, 420, 557, 137);
        init_color(COLOR_MAPLE_2, 546, 0, 0);
        init_color(COLOR_MAPLE_3, 1000, 323, 0);
        init_pair(COLOR_LEAF_BLACK, COLOR_LEAF, COLOR_BLACK);
        init_pair(COLOR_WOOD_BLACK, COLOR_WOOD, COLOR_BLACK);
        init_pair(COLOR_CHERRYBLOSSOM_1_BLACK, COLOR_CHERRYBLOSSOM_1, COLOR_BLACK);
        init_pair(COLOR_CHERRYBLOSSOM_2_BLACK, COLOR_CHERRYBLOSSOM_2, COLOR_BLACK);
        init_pair(COLOR_CHERRYBLOSSOM_3_BLACK, COLOR_CHERRYBLOSSOM_3, COLOR_BLACK);
        init_pair(COLOR_MAPLE_1_BLACK, COLOR_MAPLE_1, COLOR_BLACK);
        init_pair(COLOR_MAPLE_2_BLACK, COLOR_MAPLE_2, COLOR_BLACK);
        init_pair(COLOR_MAPLE_3_BLACK, COLOR_MAPLE_3, COLOR_BLACK);
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
            if( selectedTab != NULL && panel->selected != 0)
            {
                *selectedTab = panel->offset + panel->selected;
                DestroyDetailPanel();
                ClearScreen();
                return DISPLAY_DETAILS;
            }
            break;

        case 'B':
        case 'b':
            return DISPLAY_TURNOFF_BUZZER;

        case 'C':
        case 'c':
            if (selectedTab != NULL)
            {
                ClearScreen();
                return DISPLAY_COLOR_SETTING;
            }
            break;

        case 'R':
        case 'r':
            return DISPLAY_REFRESh_OVERVEIW;

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

int DisplayDetails(WINDOW* windowLeft, WINDOW* windowRight, char* clientID, int colorCode, int* selectedTab)
{
    DrawPanel(windowLeft, clientDetailPanel, LEFT);
    DrawTree(windowRight, GetTree(windowRight), colorCode);

    *selectedTab = -1;
    return HandleKeyboardInput(clientDetailPanel, selectedTab);
}

int DisplayHelp(WINDOW* window, Panel* panel)
{  
    DrawPanel(window, panel, CENTER);    
    return HandleKeyboardInput(panel, NULL);
}

int DisplayColorSetting(WINDOW* window, char* clientID, int fd)
{
    if (strcmp(clientID, "") == 0)
    {
        return DISPLAY_DETAILS;
    }

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

    return DISPLAY_DETAILS;
}

void RunGUIManager(int inputPipe)
{
    InitCurses();
    DrawTitle();

    int clientInfosSize = 0;
    ClientInfos* clientInfos = NULL;
    clientInfos = GetClientInfos(clientInfos, &clientInfosSize);

    WINDOW* fullScreenWindow = newwin(LINES - MARGIN_TOP * 2, COLS - MARGIN_LEFT * 2, MARGIN_TOP, MARGIN_LEFT);
    WINDOW* cropScreenWindow = newwin(LINES / 2, COLS / 2, LINES / 4, COLS / 4);
    WINDOW* leftLopsidedWindow = newwin(LINES - MARGIN_TOP * 2, (COLS / 3) * 2 - MARGIN_LEFT * 2, MARGIN_TOP, MARGIN_LEFT);
    WINDOW* rightLopsidedWindow = newwin(LINES - MARGIN_TOP * 2, ((COLS) / 3) * 1 - MARGIN_LEFT * 2, MARGIN_TOP, (COLS / 3) * 2 + MARGIN_LEFT);

    PanelMessage* overviewMessage = NewPanelMessage(ClientInfosToString(clientInfos, clientInfosSize), clientInfosSize + 1); // +1 is header 
    PanelMessage* helpMessage = NewPanelMessage(help, HELP_MESSAGE_SIZE);
    PanelMessage* titleMessage = NewPanelMessage(title, TITLE_MESSAGE_SIZE);

    Panel* overviewPanel = wNewPanel(fullScreenWindow, MARGIN_TOP, MARGIN_LEFT, overviewMessage);
    Panel* helpPanel = wNewPanel(fullScreenWindow, MARGIN_TOP, MARGIN_LEFT, helpMessage);
    Panel* titlePanel = wNewPanel(fullScreenWindow, (LINES - MARGIN_TOP * 2 - TITLE_MESSAGE_SIZE) / 2, MARGIN_LEFT, titleMessage);
    titlePanel->selected = -1; // titlePanel just display title string;

    char clientID[MAX_CLIENT_ID_SIZE] = "";
    int selectedTab = 0;
    int displayStatus = DISPLAY_OVERVIEW;
    int displayNext;
    int clientInfosOrder = DESCENDING;

    DrawPanel(fullScreenWindow, titlePanel, CENTER);
    getch();

    while(1)
    {
        switch (displayStatus)
        {
        case DISPLAY_OVERVIEW:
            if ((displayNext = DisplayOverview(fullScreenWindow, overviewPanel, &selectedTab)) != DISPLAY_CURRENT)
            {
                displayStatus = displayNext;
            }

            if ((overviewPanel->selected + overviewPanel->offset - 1) != 0)
            {
                strcpy(clientID,clientInfos[overviewPanel->selected + overviewPanel->offset - 1].clientID); 
            }
            GetDetailPanel(leftLopsidedWindow, clientID);
            break;

        case DISPLAY_DETAILS:
            int colorCode = GetTreeColor(clientInfos[overviewPanel->selected + overviewPanel->offset - 1].species, clientInfos[overviewPanel->selected + overviewPanel->offset - 1].progress);
            if ((displayNext = DisplayDetails(leftLopsidedWindow, rightLopsidedWindow, clientID, colorCode, &selectedTab)) != DISPLAY_CURRENT)
            {
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
            if ((displayNext = DisplayColorSetting(cropScreenWindow, clientID, inputPipe)) != DISPLAY_CURRENT)
            {
                displayStatus = displayNext;
            }
            break;
        
        case DISPLAY_TURNOFF_BUZZER:
            TurnOffBuzzer(inputPipe);
            displayStatus = DISPLAY_OVERVIEW;
            break;

        case DISPLAY_REFRESh_OVERVEIW:
            clientInfos = GetClientInfos(clientInfos, &clientInfosSize);
            overviewMessage = NewPanelMessage(ClientInfosToString(clientInfos, clientInfosSize), clientInfosSize + 1); // +1 is header 
            overviewPanel = wNewPanel(fullScreenWindow, MARGIN_TOP, MARGIN_LEFT, overviewMessage);
            mvwprintw(stdscr, LINES - 1, 0, "Refresh data . Press any key to continue");
            getch();
            ClearScreen();
            displayStatus = DISPLAY_OVERVIEW;
            break;

        default:
            ASSERT(displayStatus && FALSE, "undefined displayStatus");
            displayStatus = DISPLAY_OVERVIEW;
            break;
        }
    }
}