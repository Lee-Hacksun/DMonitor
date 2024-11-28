#ifndef TREE_H
#define TREE_H

#include <ncurses.h>

#include "TabPanel.h"

Panel* GetTree(WINDOW* window);
void DrawTree( WINDOW* window, Panel* panel, int colorPairLeaf, int colorPairWood);

#endif