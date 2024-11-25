#ifndef TABPANEL_H
#define TABPANEL_H

#define LEFT 1
#define CENTER 2
#define RIGHT 3

#define MARGIN_TOP 1
#define MARGIN_LEFT 1

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
    int offset;
    int selected;
    PanelMessage* panelMessage;
} Panel;

PanelMessage* NewPanelMessage(char** msg, size_t size);
Panel* NewPanel(int height, int width, int startY, int startX, PanelMessage* panelMessage);
Panel* wNewPanel(WINDOW* window, int startY, int startX, PanelMessage* PanelMessage);
void IncreasePanelOffset(Panel* panel);
void DecreasePanelOffset(Panel* panel);
void IncreasePanelSelect(Panel* panel);
void DecreasePanelSelect(Panel* panel);
void DrawPanel(WINDOW* window, Panel* panel, int AlignOption);
void DestroyPanel(Panel* panel);

#endif 