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

Panel* NewPanel(int height, int width, int startY, int startX, PanelMessage* panelMessage)
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
    localPanel->offset = 0;
    localPanel->selected = 0;
    localPanel->panelMessage = panelMessage;

    return localPanel;
}

Panel* wNewPanel(WINDOW* window, int startY, int startX, PanelMessage* panelMessage)
{
    int panelHeight;
    int panelWidth;
    getmaxyx(window, panelHeight, panelWidth);

    Panel* panel = NewPanel(panelHeight - MARGIN_TOP * 2, panelWidth - MARGIN_LEFT * 2, startY , startX + MARGIN_LEFT, panelMessage);
    return panel;
}

void IncreasePanelOffset(Panel* panel)
{
    if (panel->offset + panel->height < panel->panelMessage->size)
    {
        panel->offset++;
    }
}

void DecreasePanelOffset(Panel* panel)
{
    if (panel->offset != 0)
    {
        panel->offset--;
    }
}

void IncreasePanelSelect(Panel* panel)
{
    if (panel->selected + panel->startY == panel->height)
    {
        IncreasePanelOffset(panel);
    }
    else if (panel->selected < panel->panelMessage->size - 1)
    {
        panel->selected++;
    }
}

void DecreasePanelSelect(Panel* panel)
{
    if (panel->selected == 0)
    {
        DecreasePanelOffset(panel);
    }
    else
    {
        panel->selected--;
    }
}

// TODO : 패딩, 마진 상수로 변경
void DrawPanel( WINDOW* window, Panel* panel, int AlignOption)
{
    wclear(window);
    box(window, 0, 0);

    for (int i = panel->offset; (i < panel->panelMessage->size + panel->offset) && (i < panel->panelMessage->size); i++)
    {  
        int tabIndex = i - panel->offset;
        if (tabIndex >= panel->height)
        {
            break;
        }

        if (tabIndex == panel->selected)
        {
            wattron(window, A_REVERSE);
        }

        switch (AlignOption)
        {
        case LEFT:
            mvwprintw(window, panel->startY + tabIndex, panel->startX, "%s", panel->panelMessage->informations[i]);    
            break;
                
        case CENTER:
            mvwprintw(window, panel->startY + tabIndex, (panel->width - strlen(panel->panelMessage->informations[i])) / 2, "%s", panel->panelMessage->informations[i]);    
            break;

        case RIGHT:
            // The reason for subtracting 1 from the comparison value is because of the border of the window
            mvwprintw(window, panel->startY + tabIndex, panel->width - strlen(panel->panelMessage->informations[i]) - 1, "%s", panel->panelMessage->informations[i]);    
            break;

        default:
            ASSERT(0, "AlignOption is Invalid value")
            break;
        }

        if (tabIndex == panel->selected) 
        {
            wattroff(window, A_REVERSE);
        }
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