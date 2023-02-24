#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

// ȭ�� ��� ���� ��ɾ� ���̺귯�� ���� �ҽ��ڵ�

// **** ȭ�� ũ�� ��� ****
#define WIDTH 140
#define HEIGHT 60

// **** ���� ��� ****
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15


#define ESC 0x1b
#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.
#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'
#define F1	0x3b//���͸� ����Ͽ� ���
#define F2	0x3c
#define BLANK ' '

// **** ���� ���۸� ���� ****
int hidden_index; // Hidden ȭ�� ��ȣ 0 or 1
HANDLE scr_handle[2]; // ȭ�� ���� ����


// **** ���� �Լ� ****

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	// hidden screen�� gotoxy
	SetConsoleCursorPosition(scr_handle[hidden_index], CursorPosition);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(scr_handle[hidden_index], fg_color | bg_color << 4);
}

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void showCursor(void) { // Ŀ���� ���̰� �Ѵ�
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(scr_handle[hidden_index], &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(scr_handle[hidden_index], &curInfo);
}

void cls(int text_color, int bg_color) // ȭ�� �����
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

void ChangeScreenSize(HANDLE hnd, COORD NewSize)
{
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(hnd, &SBInfo);
	DisplayArea.Bottom = NewSize.Y;
	DisplayArea.Right = NewSize.X;
	SetConsoleWindowInfo(hnd, 1, &DisplayArea);
}

void ChangeBuffSize(HANDLE hnd, COORD NewSize)
{
	SetConsoleScreenBufferSize(hnd, NewSize);
}

// **** ���� ���۸� ���� �Լ� ****

void scr_init()
{
	CONSOLE_CURSOR_INFO cci;

	// ȭ�� ���� 2���� �����.
	scr_handle[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	scr_handle[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ���� �����.
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(scr_handle[0], &cci);
	SetConsoleCursorInfo(scr_handle[1], &cci);
	// 0�� ȭ���� default
	SetConsoleActiveScreenBuffer(scr_handle[0]);
	hidden_index = 1; // 1�� ȭ���� Hidden Screen
}

void scr_switch()
{
	// hidden �� active �� �����Ѵ�.
	SetConsoleActiveScreenBuffer(scr_handle[hidden_index]);
	// active�� hidden���� �����Ѵ�.
	hidden_index = !hidden_index; // 0 <-> 1 toggle
}

void scr_clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	// hidden screen�� clear�Ѵ�.
	// WIDTH*2 * HEIGHT ���� [�Ӽ�]���� ������ ���� ��Ȯ�� ���ƾ� �Ѵ�.
	// ��, ȭ�� �Ӽ����� �ʺ�(W)=80, ����(H)=25��� Ư�� ���ڴ� 2ĭ�� �̹Ƿ� WIDTH=40, HEIGHT=25�̴�.
	FillConsoleOutputCharacter(scr_handle[hidden_index], ' ', WIDTH * 2 * HEIGHT, Coor, &dw);
}

void scr_release()
{
	CloseHandle(scr_handle[0]);
	CloseHandle(scr_handle[1]);
}

void printscr(const char* str)
{
	// const char*������ �Ű������� ������.
	DWORD dw;
	// hidden screen�� gotoxy �Ǿ��ٰ� �����ϰ� print
	WriteFile(scr_handle[hidden_index], str, strlen(str), &dw, NULL);
}

void printintscr(int str)
{
	// const char*������ �Ű������� ������.
	DWORD dw;
	// hidden screen�� gotoxy �Ǿ��ٰ� �����ϰ� print
	WriteFile(scr_handle[hidden_index], str, strlen(str), &dw, NULL);
}

void printxy(int x, int y, char* str)
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	// hidden screen�� gotoxy + print
	SetConsoleCursorPosition(scr_handle[hidden_index], CursorPosition);
	WriteFile(scr_handle[hidden_index], str, strlen(str), &dw, NULL);
}