#include <string.h>
#include <ncurses.h>

#include "DebugUtil.h"
#include "GUIManager.h"
#include "TabPanel.h"
#include "Tree.h"

Panel* GetTree(WINDOW* window)
{
    char* asciiTree[] = {
    "           &&& &&  & &&          ",
    "      && &\\/&\\|& ()|/ @, &&      ",
    "      &\\/(/&/&||/& /_/)_&/_&      ",
    "   &() &\\/&|()|/&\\/ '%\" & ()     ",
    "  &_\\_&&_\\ |& |&&/&__%_/_& &&     ",
    "&&   && & &| &| /& & % ()& /&&    ",
    " ()&_---()&\\&\\|&&-&&--%---()~    ",
    "            \\|||                  ",
    "             |||                  ",
    "             |||                  ",
    "             |||                  ",
    "       , -=-~  .-^- _            ",
    "             `                    "
    };

    PanelMessage* treeMessage = NewPanelMessage(asciiTree, 13);
    Panel* treePanel = wNewPanel(window, MARGIN_TOP, MARGIN_LEFT, treeMessage); 
    treePanel->selected = -1;

    return treePanel;
}

void DrawTree( WINDOW* window, Panel* panel, int colorPairLeaf)
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

        if (tabIndex >= 0 && tabIndex < 7)
        {
            wattron(window, COLOR_PAIR(colorPairLeaf));
        }
        else if(tabIndex >= 7 && 13 > tabIndex)
        {
            wattron(window, COLOR_PAIR(COLOR_WOOD_BLACK));
        }

        mvwprintw(window, panel->startY + tabIndex, (panel->width - strlen(panel->panelMessage->informations[i])) / 2, "%s", panel->panelMessage->informations[i]);    

        if (tabIndex >= 0 && tabIndex < 7) 
        {
            wattroff(window, COLOR_PAIR(colorPairLeaf));
        }
        else if(tabIndex >= 7 && 13 > tabIndex)
        {
            wattroff(window, COLOR_PAIR(COLOR_WOOD_BLACK));
        }
    }

    wrefresh(window);
}
