#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "DebugUtil.h"
#include "TabPanel.h"
 
PanelMessage* NewPanelMessage(char** msg, size_t size)
{
    ASSERT(msg != NULL, "At least one message must exist.")
    ASSERT(size > 0, "Panel size must be greater than 0.");

    PanelMessage* panelMessage = (PanelMessage*)malloc(sizeof(PanelMessage));

    panelMessage->size = size;
    panelMessage->informations = (char**)malloc(sizeof(char*) * size);
    if (panelMessage->informations == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        panelMessage->informations[i] = (char*)malloc(sizeof(char) * (strlen(msg[i]) + 1)); // +1 for '\0' 
        if (panelMessage->informations == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(panelMessage->informations[j]);
            }
            free(panelMessage->informations);
            free(panelMessage);
            return NULL;
        }
        strcpy(panelMessage->informations[i], msg[i]);
    }

    return panelMessage;
}

Panel* NewPanNel(int height, int width, int startY, int startX, PanelMessage* panelMessage)
{
    ASSERT(height > -1, "height must be positive.");
    ASSERT(width > -1, "width must be positive.");
    ASSERT(startY > -1, "startY must be posotive.");
    ASSERT(startX > -1, "startX must be posotive.");
    ASSERT(panelMessage != NULL, "At least one message must exist.")

    Panel* localPanel = (Panel*)malloc(sizeof(Panel));
    memset(localPanel, 0, sizeof(Panel));
    localPanel->height = height;
    localPanel->width = width;
    localPanel->startY = startY;
    localPanel->startX = startX;
    localPanel->panelMessage = panelMessage;

    return localPanel;
}

// TODO : 선택(하이라이트) 구현 
void DrawPanel(Panel* panel, WINDOW* window)
{
    int maxY = 0;
    int maxX = 0;

    getmaxyx(window, maxY, maxX);
    

    for (int i = 0; i < panel->panelMessage->size; i++)
    {
        if (panel->startY + i == (maxY - 2))
        {
            // TODO : pre, next 선택지 구현
            break;
        }
        mvwprintw(window, panel->startY + i, panel->startX, "%s", panel->panelMessage->informations[i]);
    }

    wrefresh(window);
}

void DestroyPanelMessage(PanelMessage* panelMessage)
{
    for (int i = 0; i < panelMessage->size; i++)
    {
        free(panelMessage->informations[i]);
    }
    free(panelMessage->informations);
}

void DestroyPanel(Panel* panel)
{
    DestroyPanelMessage(panel->panelMessage);
    free(panel);
} 