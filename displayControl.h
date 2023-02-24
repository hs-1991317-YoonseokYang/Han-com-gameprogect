#ifndef DISPLAYCONTROL_H
#define DISPLAYCONTROL_H

#include <stdio.h>
#include <Windows.h>

// ȭ�� ��� ���� ��ɾ� ���̺귯��

//SetConsoleTitle(TEXT("Quick Draw"));
//system("mode con cols=120 lines=40");

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

//Ű���� ���� ���
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

// **** ���� �Լ� ****
void gotoxy(int x, int y);

void textcolor(int fg_color, int bg_color);

void removeCursor(void);

void showCursor(void);

void cls(int text_color, int bg_color);

void ChangeScreenSize(HANDLE hnd, COORD NewSize);

void ChangeBuffSize(HANDLE hnd, COORD NewSize);

// **** ���� ���۸� ���� �Լ� ****

void scr_init();

void scr_switch();

void scr_clear();

void scr_release();

void printscr(const char* str);

void printintscr(int str);

void printxy(int x, int y, char* str);

#endif