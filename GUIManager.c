#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "DebugUtil.h"
#include "GUIManager.h"
#include "TabPanel.h"

void InitCurses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();
    refresh();
}

// 로그 파일 읽기()

int DisplayOverview()
{
    clear();
    /*
    로그 파일 읽기
    패널 그리기
    while(1)
    {
        입력 받기
        패널 그리기
        if 선택 || 이동 || 새로고침
        {
            코드 반환
        }
    }
    */
}

int DisplayDetails()
{
    clear();
    /*
    로그 파일 읽기
    선택된 정보 그리기
    while(1)
    {
        입력 받기
        if (나가기 || 색상 등록 || 부저 컨트롤)
        {
            코드 반환
        }
    }
    */
}

void RunGUIManager()
{
    #if TEST

    InitCurses();

    int height = LINES - 2;
    int width = COLS - 6;
    int startY = 1;
    int startX = 0;

    WINDOW* my_win;
    my_win = newwin(height, width, startY, startX);
    box(my_win, 0, 0);
    wrefresh(my_win);
    
    char* msg = ">> TabPanel <<";
    mvwprintw(stdscr, 0, (COLS - strlen(msg)) /2, "%s", msg);
    
    char* information[] = 
    {
        "Hello!",
        "World",
        "BYE",
        "Hello!",
        "World",
        "BYE",
        "Hello!",
        "World",
        "BYE",
        "Hello!",
        "World",
        "BYE",
        "Hello!",
        "World",
        "BYE"
    }; 

    PanelMessage* panelMessage = NewPanelMessage(information, 15);
    Panel* panel = NewPanNel(10, width, startY, startX + 2, panelMessage);
    DrawPanel(panel, my_win);
    
    wrefresh(my_win);
    getch();
    endwin();

    #else
    int displayStatus = DISPLAY_OVERVIEW;

    while(1)
    {
        switch (displayStatus)
        {
        case DISPLAY_OVERVIEW:
            displayStatus = DisplayOverview();
            break;

        case DISPLAY_DETAILS:
            displayStatus = DisplayDetails();        
            break;
            
        default:
            ASSERT(displayStatus && FALSE, "undefined displayStatus");
            displayStatus = DISPLAY_OVERVIEW;
            break;
        }
    }

    #endif
}