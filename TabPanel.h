#ifndef TABPANEL_H
#define TABPANEL_H

typedef struct _panelMessage
{
    char** informations;
    int size;
} PanelMessage;

typedef struct _panel
{
    int width;
    int height;
    int startX;
    int startY;
    PanelMessage* panelMessage;
} Panel;

PanelMessage* NewPanelMessage(char** msg, size_t size);
Panel* NewPanNel(int height, int width, int startY, int startX, PanelMessage* panelMessage);
void DrawPanel(Panel* panel, WINDOW* window);
void DestroyPanel(Panel* panel);

#endif 