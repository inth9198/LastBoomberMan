#pragma warning(disable : 4996)

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <process.h>

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
#define GBOARD_ORIGIN_X 16
#define GBOARD_ORIGIN_Y 10

int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];     // 충돌검사용 필드
int front_buffer[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

int LIFE;
int bluezonecount;
int itemCount = 0;
int isCollidedRedHole = 0;
int appliedItem = 0;
double TIME;
int SCORE;

void RemoveCursor();
void DrawGameTitle();
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
void InstallBomb();
void makeBluehole();
void nextBluehole(int n);
void DrawInfo();
void makeFirstRedHole();
void makeSecondRedHole();
void makeRedHoleToNormal();
void PrintOneBlock(int k);
unsigned _stdcall StartGame();
unsigned _stdcall DrawInfo1();
void gameover();
void printnum(int n);
void printscore(int num);
void makeItem(int y, int x);
void checkItem(int i);

int main()
{
	int n = 0;

	system("mode con cols=110 lines=40");
	system("title LAST BOMBERMAN");
	RemoveCursor();
	DrawGameTitle();

	GameBoardInfoinitial();     //보드배열 초기화

	DrawGameBoard();

	DrawOneStage();
	LIFE = 3;
	TIME = 60;
	SCORE = 0;
	appliedItem = 0;
	itemCount = 0;
	//Sleep(1000);
	RedrawBoard();

	//_beginthreadex(NULL, 0, DrawInfo1, 0, 0, NULL);
	//_beginthreadex(NULL, 1000, StartGame, 0, 0, NULL);

	while (1)
	{
		ProcessKeyInput();
		nextBluehole(n);
		n++;
		if (LIFE <= 0)
			break;
	}
	Sleep(2000);
	gameover();
	getchar();              //종료전 대기시간 화면에 글자 보기 싫어서 만들었습니다.

	return 0;
}

unsigned _stdcall StartGame() {
	int n = 0;

	while (1)
	{
		ProcessKeyInput();
		nextBluehole(n);
		n++;
	}
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


void DrawGameTitle() {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

	printf("\n");
	printf("    ■    ■■  ■■■ ■■■  ■■     ■■   ■■  ■■ ■■   ■■■ ■■    ■■  ■■  ■■  ■    ■\n");
	printf("    ■   ■  ■ ■       ■    ■  ■ ■    ■ ■ ■■ ■ ■  ■ ■     ■ ■   ■ ■■ ■ ■  ■ ■■  ■\n");
	printf("    ■   ■■■ ■■■   ■    ■■   ■    ■ ■  ■  ■ ■■   ■■   ■■    ■  ■  ■ ■■■ ■ ■ ■\n");
	printf("    ■   ■  ■     ■   ■    ■  ■ ■    ■ ■  ■  ■ ■  ■ ■     ■ ■   ■  ■  ■ ■  ■ ■  ■■\n");
	printf("    ■■ ■  ■ ■■■   ■    ■■■   ■■   ■      ■ ■■■ ■■■ ■  ■  ■      ■ ■  ■ ■    ■");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

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
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
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

	SetCurrentCursorPos(GBOARD_ORIGIN_X + 12, GBOARD_ORIGIN_Y + 5);
	printf("");
	SetCurrentCursorPos(GBOARD_ORIGIN_X + 12, GBOARD_ORIGIN_Y + 5);

}

void GameBoardInfoinitial()
{
	int x, y;

	for (y = 0; y < GBOARD_HEIGHT; y++)             // 일단 전부 0으로 초기화 
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
	makeBluehole();
}

void BlockRight()
{
	COORD curPos = GetCurrentCursorPos();

	if (!DetectCollison(curPos.X + 2, curPos.Y))
		return;

	int arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
	int arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

	if (gameBoardInfo[arrCurY][arrCurX] != 0) {
		PrintOneBlock(gameBoardInfo[arrCurY][arrCurX]);
	}
	else {
		printf("  ");
	}

	curPos.X += 2;
	SetCurrentCursorPos(curPos.X, curPos.Y);
	printf("");
	SetCurrentCursorPos(curPos.X, curPos.Y);

}

void BlockLeft()
{
	COORD curPos = GetCurrentCursorPos();

	if (!DetectCollison(curPos.X - 2, curPos.Y))
		return;

	int arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
	int arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

	if (gameBoardInfo[arrCurY][arrCurX] != 0) {
		PrintOneBlock(gameBoardInfo[arrCurY][arrCurX]);
	}
	else {
		printf("  ");
	}

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


	int arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
	int arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

	if (gameBoardInfo[arrCurY][arrCurX] != 0) {
		PrintOneBlock(gameBoardInfo[arrCurY][arrCurX]);
	}
	else {
		printf("  ");
	}

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

	int arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
	int arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

	if (gameBoardInfo[arrCurY][arrCurX] != 0) {
		PrintOneBlock(gameBoardInfo[arrCurY][arrCurX]);
	}
	else {
		printf("  ");
	}

	curPos.Y += 1;
	SetCurrentCursorPos(curPos.X, curPos.Y);

	printf("");
	SetCurrentCursorPos(curPos.X, curPos.Y);

}

void InstallBomb()
{
	COORD curPos = GetCurrentCursorPos();

	int i = 1;
	while (i < 20) {

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i++);
		printf("■");
		Sleep(20);
		SetCurrentCursorPos(curPos.X, curPos.Y);
		i++;
	}

	SetCurrentCursorPos(curPos.X - 2, curPos.Y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
	printf("■■■");

	SetCurrentCursorPos(curPos.X, curPos.Y - 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
	printf("■");

	SetCurrentCursorPos(curPos.X, curPos.Y + 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
	printf("■");

	Sleep(100);

	TIME -= 0.5;

	int arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
	int arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

	SetCurrentCursorPos(curPos.X - 2, curPos.Y);
	for (int i = -1; i <= 1; i++) {
		PrintOneBlock(gameBoardInfo[arrCurY][arrCurX + i]);
	}

	SetCurrentCursorPos(curPos.X, curPos.Y - 1);
	PrintOneBlock(gameBoardInfo[arrCurY - 1][arrCurX]);

	SetCurrentCursorPos(curPos.X, curPos.Y + 1);
	PrintOneBlock(gameBoardInfo[arrCurY + 1][arrCurX]);

	if (gameBoardInfo[arrCurY][arrCurX - 1] == 2)
	{
		gameBoardInfo[arrCurY][arrCurX - 1] = 0;
		SCORE += 10;
		makeItem(arrCurY, arrCurX - 1);
	}
	if (gameBoardInfo[arrCurY][arrCurX + 1] == 2)
	{
		gameBoardInfo[arrCurY][arrCurX + 1] = 0;
		SCORE += 10;
		makeItem(arrCurY, arrCurX + 1);
	}
	if (gameBoardInfo[arrCurY - 1][arrCurX] == 2)
	{
		gameBoardInfo[arrCurY - 1][arrCurX] = 0;
		SCORE += 10;
		makeItem(arrCurY - 1, arrCurX );
	}
	if (gameBoardInfo[arrCurY + 1][arrCurX] == 2)
	{
		gameBoardInfo[arrCurY + 1][arrCurX] = 0;
		SCORE += 10;
		makeItem(arrCurY + 1 , arrCurX );
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("");
	SetCurrentCursorPos(curPos.X, curPos.Y);

	RedrawBoard();
}


void makeItem(int y, int x)
{
	int r = rand() % 20;
	if (r == 1)
		gameBoardInfo[y][x] = 6;
	else if (r == 2)
		gameBoardInfo[y][x] = 7;
	else if (r == 3)
		gameBoardInfo[y][x] = 8;
	else if (r == 4)
		gameBoardInfo[y][x] = 9;
}

int DetectCollison(int curX, int curY)
{
	int arrX = (curX - GBOARD_ORIGIN_X) / 2;
	int arrY = curY - GBOARD_ORIGIN_Y;

	if ((gameBoardInfo[arrY][arrX] != 1) && (gameBoardInfo[arrY][arrX] != 2))
		return 1;
	return 0;                   //문제가 없으면 1 반환
}

void DrawInfo()
{
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 3);
	printf("LIFE = %d", LIFE);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 9);
	printf("TIME = %.f", TIME);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 15);
	printf("SCORE = %d", (int)(60 - TIME) + SCORE);
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 19);
	printf("ITEM");
	if (appliedItem == 6)
		printf(" = ◆");
	else if (appliedItem == 7)
		printf(" = ♡");
	else if (appliedItem == 8)
		printf(" = ♤");
	else if (appliedItem == 9)
		printf(" = ♧");
	else
		printf("     ");
}

unsigned _stdcall DrawInfo1()
{
	while (1) {
		COORD  curPos = GetCurrentCursorPos();
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 3);
		printf("LIFE = %d", LIFE);
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 9);
		printf("TIME = %.2f", TIME);
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 15);
		printf("SCORE = %d", (int)(60 - TIME) + SCORE);
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 5, GBOARD_ORIGIN_Y + 19);
		printf("ITEM");
		TIME -= 1;
		SetCurrentCursorPos(curPos.X, curPos.Y);
		Sleep(1000);

	}
}

void checkItem(int i)
{
	COORD curPos = GetCurrentCursorPos();

	if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 6 &&
		appliedItem != 6)
	{
		appliedItem = 6;
		gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] = 0;
		itemCount = 0;
	}
	else if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 7 &&
		appliedItem != 7)
	{
		appliedItem = 7;
		gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] = 0;
		itemCount = 0;
		LIFE++;
	}
	else if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 8 &&
		appliedItem != 8)
	{
		appliedItem = 8;
		gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] = 0;
		itemCount = 0;
	}
	else if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 9 &&
		appliedItem != 9)
	{
		appliedItem = 9;
		gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] = 0;
		itemCount = 0;
	}
	else
	{
		itemCount++;
	}
	if (itemCount > 500)
	{
		appliedItem = 0;
		itemCount = 0;
	}
}

void ProcessKeyInput()
{
	COORD curPos;
	int key, i;
	isCollidedRedHole = 0;

	for (i = 0; i < 500; i++)
	{
		if (LIFE <= 0)
			return;

		if (_kbhit() != 0 && appliedItem != 9 && appliedItem != 8)
		{
			key = _getch();

			switch (key)
			{
			case LEFT:
				BlockLeft();
				RedrawBoard();
				break;
			case RIGHT:
				BlockRight();
				RedrawBoard();
				break;
			case UP:
				BlockUp();
				RedrawBoard();
				break;
			case DOWN:
				BlockDown();
				RedrawBoard();
				break;
			case SPACE:
				InstallBomb();
				break;
			default:
				break;
			}
		}
		else if (_kbhit() != 0 && appliedItem == 9)
		{
			key = _getch();

			switch (key)
			{
			case LEFT:
				BlockRight();
				RedrawBoard();
				break;
			case RIGHT:
				BlockLeft();
				RedrawBoard();
				break;
			case UP:
				BlockDown();
				RedrawBoard();
				break;
			case DOWN:
				BlockUp();
				RedrawBoard();
				break;
			case SPACE:
				InstallBomb();
				break;
			default:
				break;
			}
		}
		else if (_kbhit() != 0 && appliedItem == 8)
		{
			key = _getch();

			switch (key)
			{
			case LEFT:
				BlockLeft();
				BlockLeft();
				RedrawBoard();
				break;
			case RIGHT:
				BlockRight();
				BlockRight();
				RedrawBoard();
				break;
			case UP:
				BlockUp();
				BlockUp();
				RedrawBoard();
				break;
			case DOWN:
				BlockDown();
				BlockDown();
				RedrawBoard();
				break;
			case SPACE:
				InstallBomb();
				break;
			default:
				break;
			}
		}
		checkItem(i);

		curPos = GetCurrentCursorPos();

		if (i == 100) {
			makeFirstRedHole();
			RedrawBoard();
		}
		else if (i == 200) {
			makeSecondRedHole();
			RedrawBoard();
		}
		else if (i > 200 && i < 230) {
			// detect collision
			// 한분기에 한번만 체크하기위해.
			if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 4 &&
				isCollidedRedHole == 0 && appliedItem != 6)
			{
				LIFE--;
				isCollidedRedHole = 1;
			}
		}
		else if (i == 230) {
			makeRedHoleToNormal();
			RedrawBoard();
		}
		printf("");

		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 2) * 2 + 10, GBOARD_ORIGIN_Y + 3);
		if (gameBoardInfo[curPos.Y - GBOARD_ORIGIN_Y][(curPos.X - GBOARD_ORIGIN_X) / 2] == 5 && appliedItem != 6)
			bluezonecount++;
		else
			bluezonecount = 0;
		if (bluezonecount == 100)
		{
			LIFE--;
			bluezonecount = 0;
		}
		DrawInfo();
		SetCurrentCursorPos(curPos.X, curPos.Y);
		Sleep(10);
		TIME -= 0.02;
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

			// 변화가있을 때만 출력.
			if (front_buffer[y][x] == gameBoardInfo[y][x])
				continue;

			SetCurrentCursorPos(cursX, cursY);

			int k = gameBoardInfo[y][x];
			PrintOneBlock(k);

			front_buffer[y][x] = k;
		}
	}

	SetCurrentCursorPos(pos.X, pos.Y);
	printf("");
	SetCurrentCursorPos(pos.X, pos.Y);
}

void PrintOneBlock(int k) {
	if (k == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("■");
	}
	else if (k == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("□");
	}
	else if (k == 3)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("Δ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 4)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		printf("■");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 5)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		printf("■");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 6)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("◆");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 7)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("♡");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 8)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("♤");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else if (k == 9)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("♧");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else
	{
		printf("  ");
	}
}

void DrawOneStage()
{
	int i, j;

	for (int i = 2; i < GBOARD_HEIGHT - 1; i++)
	{
		for (int j = 2; j < GBOARD_WIDTH; j++)
		{
			if (i % 2 == 0)
				gameBoardInfo[i][j] = 2;
			if (i % 3 == 0)
				gameBoardInfo[i][j] = 2;
			if (j % 3 == 0)
				gameBoardInfo[i][j] = 2;
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

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			gameBoardInfo[5 + i - 2][7 + j - 3] = 0;
		}
	}

}

void makeBluehole()
{
	int x, y;


	for (y = 1; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][1] = 5;
		gameBoardInfo[y][GBOARD_WIDTH] = 5;
	}
	for (x = 1; x <= GBOARD_WIDTH; x++)
	{
		gameBoardInfo[1][x] = 5;
		gameBoardInfo[GBOARD_HEIGHT - 1][x] = 5;
	}
}

void nextBluehole(int n)
{
	int x, y;


	for (y = 1; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][1 + n] = 5;
		gameBoardInfo[y][GBOARD_WIDTH - n] = 5;
	}
	for (x = 1; x <= GBOARD_WIDTH; x++)
	{
		gameBoardInfo[1 + n][x] = 5;
		gameBoardInfo[GBOARD_HEIGHT - 1 - n][x] = 5;
	}
	RedrawBoard();
}

void makeFirstRedHole()
{
	int x, y;

	for (y = 1; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x <= GBOARD_WIDTH; x++)
		{
			if (gameBoardInfo[y][x] == 0) {
				int r = rand() % 10;
				if (r == 2)
					gameBoardInfo[y][x] = 3;
			}
		}
	}

}



void makeSecondRedHole()
{
	int x, y;


	for (y = 1; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x <= GBOARD_WIDTH; x++)
		{
			if (gameBoardInfo[y][x] == 3) {

				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if (gameBoardInfo[y + i][x + j] == 0 || gameBoardInfo[y + i][x + j] == 3)
							gameBoardInfo[y + i][x + j] = 4;
					}
				}

			}
		}
	}
}

void makeRedHoleToNormal()
{
	int x, y;


	for (y = 1; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x <= GBOARD_WIDTH; x++)
		{
			if (gameBoardInfo[y][x] == 4) 
			{
				gameBoardInfo[y][x] = 0;
			}
		}
	}
}


void gameover()
{
	int x, y;
	int cursX, cursY;
	COORD pos;

	for (y = 0; y < GBOARD_HEIGHT + 1; y++)
	{
		for (x = 0; x < GBOARD_WIDTH + SIDE_WIDTH + 10; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;

			SetCurrentCursorPos(cursX, cursY);
			printf("  ");
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);

	SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y);
	printf("\n");
	printf("    ■■■■    ■   ■■  ■■ ■■■     ■■   ■      ■ ■■■ ■■  \n");
	printf("    ■        ■  ■ ■ ■■ ■ ■       ■    ■  ■    ■  ■     ■  ■\n");
	printf("    ■  ■■  ■■■ ■  ■  ■ ■■■   ■    ■   ■  ■   ■■■ ■■  \n");
	printf("    ■    ■  ■  ■ ■      ■ ■       ■    ■    ■■    ■     ■ ■ \n");
	printf("    ■■■■  ■  ■ ■      ■ ■■■     ■■       ■     ■■■ ■  ■");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	printf("\n\n");
	pos = GetCurrentCursorPos();
	printf("    ■■■   ■■   ■■  ■■   ■■■     \n");
	printf("    ■      ■  ■ ■  ■ ■  ■ ■       ■\n");
	printf("    ■■■  ■     ■  ■ ■■   ■■■     \n");
	printf("        ■  ■  ■ ■  ■ ■ ■  ■       ■\n");
	printf("    ■■■   ■■   ■■  ■  ■ ■■■    ");
	SetCurrentCursorPos(pos.X + 46, pos.Y);

	printscore((int)(60 - TIME) + SCORE);


}

void printnum(int n)
{
	COORD pos = GetCurrentCursorPos();
	switch (n)
	{
	case 0:
		printf("■■■");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■  ■");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■  ■");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("■  ■");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 1:
		printf("    ■");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("    ■");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("    ■");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("    ■");
		break;
	case 2:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("■    \n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 3:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 4:
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("    ■");
		break;
	case 5:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■    \n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 6:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■    \n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 7:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("    ■");
		break;
	case 8:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("■■■");
		break;
	case 9:
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 1);
		printf("■  ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 2);
		printf("■■■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 3);
		printf("    ■\n");
		SetCurrentCursorPos(pos.X, pos.Y + 4);
		printf("    ■");
		break;
	default:
		break;
	}
	SetCurrentCursorPos(pos.X + 7, pos.Y);
	return;
}

void printscore(int num)
{
	int a, b;
	if (num == 0)

		return;

	if (num > 0)
	{
		a = num / 10;
		b = num % 10;
		printscore(a);
		printnum(b);
	}

}