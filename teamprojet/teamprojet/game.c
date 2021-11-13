#pragma warning(disable : 4996)

#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

/*Size of gameboard*/
#define GBOARD_WIDTH 26
#define GBOARD_HEIGHT 26
#define SIDE_WIDTH 10

/*Starting point of gameboard*/
#define GBOARD_ORIGIN_X 4
#define GBOARD_ORIGIN_Y 2

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];     // �浹�˻�� �ʵ�


void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos(void);

void RemoveCursor();
void DrawGameBoard();
void GameBoardInfoinitial();
void BlockRight();
void BlockLeft();
void BlockUp();
void BlockDown();
int DetectCollison(int curX, int curY);
void ProcessKeyInput();
void RedrawBoard();
void DrawOneStage();

int main()
{
    int i;
    RemoveCursor();

    GameBoardInfoinitial();     //����迭 �ʱ�ȭ

    DrawGameBoard();

    DrawOneStage();

    RedrawBoard();

    while (1)
    {
        ProcessKeyInput();
    }

    getchar();              //������ ���ð� ȭ�鿡 ���� ���� �Ⱦ ��������ϴ�.

    return 0;
}

void SetCurrentCursorPos(int x, int y)
{
    COORD pos = { x , y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos()
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

    //���κ���
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("��");
        else if (y == 0)
            printf("��");
        else
            printf("��");
    }
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("��");
        else if (y == 0)
            printf("��");
        else
            printf("��");
    }
    for (x = 1; x < GBOARD_WIDTH + 1; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y);
        printf("��");
    }
    for (x = 1; x < GBOARD_WIDTH + 1; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("��");
    }

    //���̵�
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("��");
        else if (y == 0)
            printf("��");
        else
            printf("��");
    }
    for (y = 0; y <= GBOARD_HEIGHT; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + SIDE_WIDTH * 2, GBOARD_ORIGIN_Y + y);

        if (y == GBOARD_HEIGHT)
            printf("��");
        else if (y == 0)
            printf("��");
        else
            printf("��");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y);
        printf("��");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("��");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4);
        printf("��");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4 * 2);
        printf("��");
    }
    for (x = 1; x < SIDE_WIDTH; x++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 4 * 3);
        printf("��");
    }

    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH - 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2);
    printf("");
    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH - 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2);
}

void GameBoardInfoinitial()
{
    int x, y;

    for (y = 0; y < GBOARD_HEIGHT + 1; y++)             // �ϴ� ���� 0���� �ʱ�ȭ 
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
        gameBoardInfo[0][x] = 1;
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }
}

void BlockRight()
{
    COORD curPos = GetCurrentCursorPos();

    if (!DetectCollison(curPos.X + 2, curPos.Y))
        return;

    printf("  ");
    curPos.X += 2;
    printf("");
    SetCurrentCursorPos(curPos.X, curPos.Y);
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void BlockLeft()
{
    COORD curPos = GetCurrentCursorPos();

    if (!DetectCollison(curPos.X - 2, curPos.Y))
        return;

    printf("  ");
    curPos.X -= 2;
    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("");
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void BlockUp()
{
    COORD curPos = GetCurrentCursorPos();

    if (!DetectCollison(curPos.X, curPos.Y - 1))
        return;

    printf("  ");
    curPos.Y -= 1;
    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("");
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void BlockDown()
{
    COORD curPos = GetCurrentCursorPos();

    if (!DetectCollison(curPos.X, curPos.Y + 1))
        return;

    printf("  ");
    curPos.Y += 1;
    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("");
    SetCurrentCursorPos(curPos.X, curPos.Y);
}

void Blockout()
{
    COORD curPos = GetCurrentCursorPos();

    SetCurrentCursorPos(curPos.X, curPos.Y);
    printf("  ");
}


int DetectCollison(int curX, int curY)
{
    int arrX = (curX - GBOARD_ORIGIN_X) / 2;
    int arrY = curY - GBOARD_ORIGIN_Y;

    if (gameBoardInfo[arrY][arrX] == 0)
        return 1;
    return 0;                   //������ ������ 1 ��ȯ
}


void ProcessKeyInput()
{
    int key, i;

    for (i = 0; i < 100; i++)
    {
        if (_kbhit() != 0)
        {
            key = _getch();

            switch (key)
            {
            case LEFT:
                BlockLeft();
                break;
            case RIGHT:
                BlockRight();
                break;
            case UP:
                BlockUp();
                break;
            case DOWN:
                BlockDown();
                break;
            case SPACE:
                Blockout();
                break;
            default:
                break;
            }
        }
        //Sleep(10);
    }

}

void RedrawBoard()
{
    COORD pos;
    int x, y;
    int cursX, cursY;

    pos = GetCurrentCursorPos();

    for (y = 1; y < GBOARD_HEIGHT; y++)
    {
        for (x = 1; x < GBOARD_WIDTH + 1; x++)
        {
            cursX = x * 2 + GBOARD_ORIGIN_X;
            cursY = y + GBOARD_ORIGIN_Y;
            SetCurrentCursorPos(cursX, cursY);
            if (gameBoardInfo[y][x] == 1)
            {
                printf("■");
            }
            else if (gameBoardInfo[y][x] == 2)
            {
                printf("□");
            }
            else { printf("  "); }
        }
    }

    SetCurrentCursorPos(pos.X, pos.Y);
    printf("");
    SetCurrentCursorPos(pos.X, pos.Y);
}

void DrawOneStage()
{
    int i, j;
    for (int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for (int j = 2; j < GBOARD_HEIGHT; j++)
        {
            if (i % 2 == 0)
                gameBoardInfo[i][j] = 2;
            if (i % 3 == 0)
                gameBoardInfo[i][j] = 2;
            if (j % 3 == 0)
                gameBoardInfo[i][j] = 2;
        }
    }
    for (int i = 2; i < GBOARD_HEIGHT; i++)
    {
        for (int j = 2; j < GBOARD_HEIGHT; j++)
        {
            if (i % 4 == j % 4)
                gameBoardInfo[i][j] = 1;
            if ((GBOARD_HEIGHT - i) % 5 == (j) % 5)
                gameBoardInfo[i][j] = 2;
        }
    }
    for (i = 0; i < 10; i++)
    {
        gameBoardInfo[10 + i][5] = 1;
        if ((i > 6) || (i <= 3))
            gameBoardInfo[10 + i][15] = 1;
        gameBoardInfo[10 + i][24] = 1;
    }
    for (i = 0; i < 20; i++)
    {
        if (((i > 6) || (i <= 3)) && ((i > 16) | (i <= 13)))
        {
            gameBoardInfo[20][5 + i] = 1;
            gameBoardInfo[10][5 + i] = 1;
        }
    }
    COORD curPos = GetCurrentCursorPos();
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            gameBoardInfo[curPos.Y + i - 5][curPos.X + j + 10] = 0;
        }
    }
}
