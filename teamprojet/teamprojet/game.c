#pragma warning(disable : 4996)

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define SPACE 32

/*Size of gameboard*/
#define GBOARD_WIDTH 30
#define GBOARD_HEIGHT 30
#define SIDE_WIDTH 10

/*Starting point of gameboard*/
#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];     // 충돌검사용 필드


void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos(void);

void RemoveCursor();
void DrawGameBoard();
void GameBoardInfoinitial();

int main()
{
    RemoveCursor();

    GameBoardInfoinitial();     //보드배열 초기화

    DrawGameBoard();

    getchar();              //종료전 대기시간 화면에 글자 보기 싫어서 만들었습니다.

    return 0;
}

void SetCurrentCursorPos(int x, int y)
{
    COORD pos = { x , y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos(void)
{
    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;

    return curPoint;
}

void RemoveCursor()
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}


void DrawGameBoard()
{
    int x, y;

    //메인보드
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("└");
        else if (y == 0)
            printf("┌");
        else
            printf("│");
    }
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("┘");
        else if (y == 0)
            printf("┐");
        else
            printf("│");
    }
    for (x = 1; x < GBOARD_WIDTH + 1; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("─");
    }
    for (x = 1; x < GBOARD_WIDTH + 1; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("─");
    }

    //사이드
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("└");
        else if (y == 0)
            printf("┌");
        else
            printf("│");
    }
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + SIDE_WIDTH * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("┘");
        else if (y == 0)
            printf("┐");
        else
            printf("│");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y);
        printf("─");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 +x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("─");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4);
        printf("─");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4 * 2);
        printf("─");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4 * 3);
        printf("─");
    }

    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH - 2, GBOARD_ORIGIN_Y);
}

void GameBoardInfoinitial()
{
    int x, y;

    for (y = 0; y < GBOARD_HEIGHT + 1; y++)             // 일단 전부 0으로 초기화 
    {
        for (x = 0; x < GBOARD_WIDTH + 2; x++)
            gameBoardInfo[y][x] = 0;
    }

    for (y = 0; y < GBOARD_HEIGHT; y++)
    {
        gameBoardInfo[y][0] = 1;
        gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
    }
    for (x = 0; x < GBOARD_WIDTH + 2; x++)
    {
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }
}
