#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include "displayControl.h"
#pragma warning(disable : 4996)

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

#define STAR '*'
#define BLANK ' '

#define ESC 0x1b

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define E_MAX_BULLET 300
#define MAX_BULLET 2//2
#define MAX_ITEM 30
#define WIDTH 80
#define HEIGHT 45
#define JETX 3
#define JETY 3
#define STARTX 38
#define STARTY 43
#define SHAPE "=A="
#define SHAPE21 "MWM"
#define SHAPE22 "IvI"
#define FIRE 'z'
#define BSHAPE "*"
#define BSHAPE2 "V"
#define ISHAPE1 "[L]"
#define ISHAPE2 "[P]"
#define ISHAPE3 "+S+"
#define BOSS_START_X 33
#define BOSS_START_Y 4
#define LIFE 7;//7로하자
#define BOSS_SCORE 52
#define BOSS_LIFE 10

int keep_moving = 0;
int Delay = 30;
int ourforce_frame_sync = 1;
int enemy1_frame = 13;//29
int boss_frame = 10;
int item_frame = 15;//25
int bullet_frame = 2;
int e_bullet_frame = 3;
int e_bullet2_frame = 6;//유도탄
int frame_count = 0;
int score = 0;//적을 죽이면 오른다


typedef struct jet {

	int x;
	int y;
	int oldx, oldy;
	int life;
}jet;

typedef struct enemy {
	int exist;
	int type;
	int life;
	int x, y;
	struct enemy* next;
	//int life;
}enemy;



typedef struct bullet {

	int exist;
	int life;
	int type;
	int x, y;
	struct bullet* next;

}Bullet;

typedef struct item {
	int exist;
	int life;
	int type;
	int x, y;
	struct item* next;

}item;

typedef struct boss {
	int x, y;
	int life;
	char* shape[5];
}boss;



jet of;


void jet_bulletMove();
void jet_newBullet();
void explode();
void enemy_jet_check();
void drawbox();
void enemy_new_bullet(int x, int y);
void jet_draw();
void jet_draw2(int newx, int newy);

/*void removeCursor2(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}*/


void gotoxy2(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void textcolor2(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void cls2(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}


void drawbox(int tc, int bc) {

	//for (int x = 0; x <= WIDTH; x += 2) {
	//	gotoxy(x, HEIGHT);
	//	printf("%s", "■");
	//	gotoxy(x, 0);
	//	printf("%s", "■");
	//}

	/*gotoxy(0, 0); printf("┌");
	gotoxy(0, HEIGHT); printf("└");
	gotoxy(WIDTH, 0); printf("┐");
	gotoxy(WIDTH, HEIGHT); printf("┘");*/

	for (int y = 0; y <= HEIGHT; y++) {
		for (int x = 2; x < WIDTH + 2; x++) {
			textcolor(bc, bc);
			gotoxy(x, y);
			printscr(" ");
		}
	}
	textcolor(tc, bc);
	for (int y = 0; y <= HEIGHT; y++) {
		gotoxy(0, y);
		printscr("│");
		gotoxy(WIDTH, y);
		printscr("│");
	}
}

void drawbox2(int tc, int bc) {


	textcolor(tc, bc);
	for (int y = 0; y <= HEIGHT; y++) {
		gotoxy(0, y);
		printscr("│");
		gotoxy(WIDTH, y);
		printscr("│");
	}
	textcolor(BLACK, WHITE);

}
void drawbox3(int tc, int bc) {


	for (int y = 0; y <= HEIGHT; y++) {
		for (int x = 2; x < WIDTH + 2; x++) {
			textcolor2(bc, bc);
			gotoxy2(x, y);
			printf(" ");
		}
	}
	textcolor2(tc, bc);
	for (int y = 0; y <= HEIGHT; y++) {
		gotoxy2(0, y);
		printf("│");
		gotoxy2(WIDTH, y);
		printf("│");
	}

}

void lose() {

	char x;
	gotoxy(38, 15);
	textcolor(0, 15);
	printscr("R E G A M E ? ( y / n )");
	scanf("%c", &x);
	if (x == 'y') {
		return;
	}
	else
		exit(0);

}
void ending() {
	int i;
	cls2(15, 0); jet_draw2(of.x, of.y); Sleep(500);
	cls2(14, 0); jet_draw2(of.x, of.y); Sleep(300);
	cls2(8, 0); jet_draw2(of.x, of.y); Sleep(300);
	cls2(7, 0); jet_draw2(of.x, of.y); Sleep(300);
	cls2(0, 14); Sleep(500);
	drawbox3(14, 0);
	textcolor2(14, 0);
	for (i = 0; i < 19; i++) {
		gotoxy2(i * 2, HEIGHT - i);
		printf("☆");
		gotoxy2(WIDTH - 1 - i * 2, HEIGHT - i);
		printf("☆");
		Sleep(300);
	}
	for (int t = 0; t < 19; t++) {
		for (int j = 2 * t; j < WIDTH - 3 - t * 2; j++) {
			gotoxy2(j + 2, HEIGHT - t);
			textcolor(15, 0);
			if (rand() % 3 == 0)
				printf("*");
			else
				printf(" ");
		}
		Sleep(100);

	}
	textcolor2(14, 0);
	gotoxy2(i * 2 + 1, HEIGHT - i);
	printf("***");
	gotoxy2(i * 2 + 1, HEIGHT - i - 1);
	printf("\a");
	printf("END");
	Sleep(5000);
	gotoxy2(40, 30);
	exit(0);
}

boss bos;
void init_boss() {
	bos.x = 25;
	bos.y = BOSS_START_Y;
	bos.life = BOSS_LIFE;
	bos.shape[0] = "IH          HI";
	bos.shape[1] = "IH  ■■■  HI";
	bos.shape[2] = "IH▒■◎■▒HI";
	bos.shape[3] = "IH  ■■■  HI";
	bos.shape[4] = "IH          HI";
}
/*void drawboss(int x, int y) {
	for (int i = 0; i < 5; i++) {
		textcolor(0, 4);
		gotoxy(x, y + i);
		printscr(bos.shape[i]);//이거 되려나??
	}
}*/
void boss_move(int oldx, int oldy, int newx, int newy) {

	/*for (int i = 0; i < 5; i++) {
		textcolor(CYAN1, CYAN1);
		gotoxy(oldx, oldy + i);
		printscr("              ");
	}*/
	for (int i = 0; i < 5; i++) {
		textcolor(0, CYAN1);
		gotoxy(newx, newy + i);
		if (i == 2)
			textcolor(4, 3);
		printscr(bos.shape[i]);
	}

}
void cls_boss(int tc, int bc) {
	static int boss_title[10][20] = {
		{1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0},
		{1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0},
		{1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
		{1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
		{1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
		{1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
		{1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
	};
	drawbox(tc, bc);
	textcolor(tc, bc);
	for (int y = 0; y < 10; y++) {
		gotoxy(20, 10 + y);
		for (int x = 0; x < 20; x++) {
			if (boss_title[y][x] == 1)
				printscr("■");
			else
				printscr("  ");
		}

	}
}
void boss_come_on_stage() {
	static int count = 0;
	static int newx, newy, oldx, oldy;
	if (count == 0) {
		newx = oldx = BOSS_START_X;
		newy = oldy = 0;
		count = 1;
	}
	printf("\a");
	scr_clear();
	for (int i = 0; i < 2; i++) {
		scr_clear();
		cls_boss(14, 12);
		scr_switch();
	}
	Sleep(800);
	for (int i = 0; i < 2; i++) {
		scr_clear();
		cls_boss(12, 9);
		scr_switch();
	}
	Sleep(500);
	printf("\a");
	for (int i = 0; i < 2; i++) {
		scr_clear();
		cls_boss(9, 4);
		scr_switch();
	}
	Sleep(800);
	for (int i = 0; i < 2; i++) {
		scr_clear();
		cls_boss(14, 0);
		scr_switch();
	}
	Sleep(500);
	printf("\a");
	for (int i = 0; i < 2; i++) {
		scr_clear();
		cls_boss(0, 4);
		scr_switch();
	}
	Sleep(2000);
	for (int i = 0; i < 2; i++) {
		scr_clear();
		drawbox(15, 3);
		scr_switch();
	}
	for (int i = 0; newy <= BOSS_START_Y;) {
		newy = oldy + 1;
		boss_move(oldx, oldy, newx, newy);

		Sleep(100);
		bos.y = oldy = newy;
	}


}

void boss_ctrl() {
	static int  oldx = BOSS_START_X, oldy = BOSS_START_Y, count = 0;
	int i;
	static int bway = 1;
	if (frame_count % boss_frame == 0) {
		i = rand() % 2;
		if (count % 10 == 0) {
			if (i == 0)
				bway = 1;
			else if (i == 1)
				bway = -1;
		}
		bos.x += bway;
		bos.y += rand() % 3 - 1;//

		if (bos.x < 3) {
			bway = -bway;
		}
		if (bos.x > WIDTH - 15) {
			bway = -bway;
		}
		if (bos.y < 0)
			bos.y += 1;
		if (bos.y > 30)
			bos.y -= 1;


		//boss_move(oldx, oldy, bos.x, bos.y);
		if (count % 5 == 4) {
			enemy_new_bullet(bos.x, bos.y + 5, 0);
			enemy_new_bullet(bos.x + 14, bos.y + 5, 0);
			enemy_new_bullet(bos.x + 7, bos.y + 3, 0);
		}


		if (count % 6 == 5) {
			enemy_new_bullet(bos.x + 7, bos.y + 3, 1);
		}
		if (count % 40 == 13 || count % 40 == 15 || count % 40 == 17 || count % 40 == 19) {
			enemy_new_bullet(bos.x, bos.y + 5, 1);
			enemy_new_bullet(bos.x + 14, bos.y + 5, 1);
			enemy_new_bullet(bos.x + 7, bos.y + 3, 1);
		}
		/*if (count % 25 == 0 ) {
			enemy_new_bullet(bos.x, bos.y + 5, 0);
			enemy_new_bullet(bos.x + 14, bos.y + 5, 0);
			enemy_new_bullet(bos.x + 7, bos.y + 3,0);
		}*/

		oldx = bos.x;
		oldy = bos.y;
		count++;
	}
	boss_move(oldx, oldy, bos.x, bos.y);
}

item ite[MAX_ITEM];
item i_head;
void init_item() {
	for (int i = 0; i < MAX_ITEM; i++) {
		ite[i].exist = 0;
		ite[i].life = 0;
		ite[i].next = NULL;
	}
}

void new_item(int n) {
	int i = 0;
	int t = 0;
	while (1) {
		i++;//이거 옮기니까 오류 났었음.
		if (ite[i].exist == 0) {
			ite[i].exist = 1;
			ite[i].life = 3;
			ite[i].next = i_head.next;
			i_head.next = &ite[i];
			t++;

		}
		if (t == n)
			break;
	}
}

void item_move() {
	static int x_way = 1;
	static int count = 0;
	if (frame_count % item_frame == 0) {
		for (item* now = i_head.next, *prev = &i_head; now != NULL; ) {

			/*textcolor(CYAN1, CYAN1);
			gotoxy(now->x, now->y);
			printscr("   ");*/

			if (now->type >= 2) {
				now->y += 1;
				if (count % 15 == 0 || now->x == 2 || now->x == WIDTH - 3)
					x_way = -x_way;
				now->x += x_way;
			}
			if (now->type == 1) {
				now->y += 1;
			}
			if (now->x<2 || now->x>WIDTH - 3 || now->y<0 || now->y>HEIGHT) {
				now->exist = 0;
				prev->next = now->next;
				now = now->next;
				continue;
			}
			if (now->life != 0) {

				gotoxy(now->x, now->y);
				if (now->type == 1) {
					textcolor(1, 14);
					printscr(ISHAPE1);
				}
				if (now->type == 2) {
					textcolor(14, 1);
					printscr(ISHAPE2);
				}
				if (now->type == 3) {
					textcolor(1, count % 6 + 10);
					printscr(ISHAPE3);
				}

				//textcolor(BLACK, WHITE);
			}

			now = now->next;
			prev = prev->next;
		}
		count++;
	}
	else {
		for (item* now = i_head.next, *prev = &i_head; now != NULL; ) {
			if (now->life != 0) {

				gotoxy(now->x, now->y);
				if (now->type == 1) {
					textcolor(14, 3);
					printscr(ISHAPE1);
				}
				if (now->type == 2) {
					textcolor(1, 3);
					printscr(ISHAPE2);
				}
				if (now->type == 3) {
					textcolor(count % 6 + 10, 3);
					printscr(ISHAPE3);
				}

				textcolor(WHITE, BLACK);
			}

			now = now->next;
			prev = prev->next;
		}
	}
}

int ic_i;
//int ic_si;
void item_ctrl() {
	int ic_t;
	static int score_fleg[250] = { 0 };
	static int count = 0;
	switch (frame_count) {
	case 150:case 500: case 1500: case 3000: case 3500: case 4500: case 6000: case 6500: case 8500: case 10500: case 12500: case 13500: case 14500: case 17500: case 20500: case 23500:

		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1;  if (rand() % 2 == 0)now->type = 1; else now->type = 3;  //1이 생명 2가 파워
			ic_t++;
			count++;
		}
		break;
	case 750: case 1550: case 3050: case 6050: case 9050:  case 13050:
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 2; //1이 생명 2가 파워
			ic_t++;
			count++;
		}
		break;
	}
	//int ic_st;

	switch (score) {
	case 5:
		if (score_fleg[5] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 2; //1이 생명 2가 파워
			ic_t++;
			score_fleg[5] = 1;
		}
		break;
	case 20:
		if (score_fleg[20] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 2; //1이 생명 2가 파워
			ic_t++;
			score_fleg[20] = 1;
		}
		break;
	case 40:
		if (score_fleg[40] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[40] = 1;
		}
		break;
	case 65:
		if (score_fleg[65] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 2; //1이 생명 2가 파워
			ic_t++;
			score_fleg[65] = 1;
		}
		break;
	case 90:
		if (score_fleg[90] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[90] = 1;
		}
		break;
	case 110:
		if (score_fleg[110] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[110] = 1;
		}
		break;
		//case 125:
		//	if (score_fleg[125] == 1)
		//		break;
		//	ic_t = 0;
		//	ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		//	new_item(ic_i);
		//	for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
		//		now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
		//		ic_t++;
		//		score_fleg[125] = 1;
		//	}
		//	break;
	case 145:
		if (score_fleg[145] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[145] = 1;
		}
		break;

		//case 160:
		//	if (score_fleg[160] == 1)
		//		break;
		//	ic_t = 0;
		//	ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		//	new_item(ic_i);
		//	for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
		//		now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
		//		ic_t++;
		//		score_fleg[160] = 1;
		//	}
		//	break;
	case 175:
		if (score_fleg[175] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[175] = 1;
		}
		break;

		//case 185:
		//	if (score_fleg[185] == 1)
		//		break;
		//	ic_t = 0;
		//	ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		//	new_item(ic_i);
		//	for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
		//		now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
		//		ic_t++;
		//		score_fleg[185] = 1;
		//	}
		//	break;
	case 200:
		if (score_fleg[200] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[200] = 1;
		}
		break;
	case 210:
		if (score_fleg[210] == 1)
			break;
		ic_t = 0;
		ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		new_item(ic_i);
		for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
			ic_t++;
			score_fleg[210] = 1;
		}
		break;
		//case 220:
		//	if (score_fleg[220] == 1)
		//		break;
		//	ic_t = 0;
		//	ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		//	new_item(ic_i);
		//	for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
		//		now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
		//		ic_t++;
		//		score_fleg[220] = 1;
		//	}
		//	break;
		//case 230:
		//	if (score_fleg[230] == 1)
		//		break;
		//	ic_t = 0;
		//	ic_i = 1;//ei와 type 설정만 해주면 다르게 나온다.
		//	new_item(ic_i);
		//	for (item* now = i_head.next; ic_t < ic_i; now = now->next) {
		//		now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 3; //1이 생명 2가 파워
		//		ic_t++;
		//		score_fleg[230] = 1;
		//	}

		//	break;

	}

	//if (frame_count % item_frame == 0)
	item_move();
}


void jet_draw(int newx, int newy) {
	textcolor(15, 3);
	gotoxy(newx, newy);
	printscr(SHAPE);
	textcolor(15, 3);
}

void jet_draw2(int newx, int newy) {

	gotoxy2(newx, newy);
	printf(SHAPE);
}

void movejet(int newx, int newy, int oldx, int oldy) {
	/*textcolor(CYAN1, CYAN1);
	gotoxy(oldx, oldy);
	printscr("   ");*/
	jet_draw(newx, newy);
}
int called = 0;
int max_bullet = MAX_BULLET;
void jet_ctrl() {

	static int oldx, oldy, newx, newy;
	int move_flag = 0;
	static unsigned char last_ch = 0;
	static unsigned char ch;
	/*static jet ourforce = { 1,STARTX,STARTY,{{0,1,0},{0,1,0},{1,0,1}} };*/
	if (!called) {
		of.oldx = of.x = oldx = newx = STARTX;
		of.oldy = of.y = oldy = newy = STARTY;

		jet_draw(newx, newy);
		called = 1;
		last_ch = 0;
		ch = 0;
	}
	if (kbhit() == 1) {  // 키보드가 눌려져 있으면
		char c1;
		c1 = getch(); // key 값을 읽는다
		if (c1 == '\0') // VS 의 NULL 값은 Error이다.
			return;//이거 오류 날 수도 ㅣㅆ음
		else
			ch = c1; // NULL이 아니면 정상 입력이다.
		// ESC 누르면 프로그램 종료
		if (ch == ESC)
			exit(0);

		if (ch == FIRE) {
			jet_newBullet(newx, newy - 1, max_bullet);
		}

		if (ch == SPECIAL1) { // 만약 특수키
			// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
			ch = getch();
			switch (ch) {
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
				last_ch = ch;
				keep_moving = 1;
				break;
			default: // 방향키가 아니면 멈춘다 1이면 계속 진행
				keep_moving = 0;
			}
		}
		else {
			// 특수 문자가 아니지만 방향키 대신 AWSD  사용하는 경우 처리
			switch (ch) {
			case UP2: last_ch = UP;
				keep_moving = 1;
				break;
			case DOWN2: last_ch = DOWN;
				keep_moving = 1;
				break;
			case RIGHT2: last_ch = RIGHT;
				keep_moving = 1;
				break;
			case LEFT2: last_ch = LEFT;
				keep_moving = 1;
				break;
			default: // 방향이 아니면 멈춘다 --> 1이면 계속 진행
				keep_moving = 0;
				break;
			}
		}

		if (keep_moving && ch == 0)
			ch = last_ch;
		//last_ch = ch;
		if (keep_moving == 1) {
			switch (last_ch) {
			case UP:
				if (oldy > 1)
					newy = oldy - 1;

				break;
			case DOWN:
				if (oldy < HEIGHT)
					newy = oldy + 1;

				break;
			case LEFT:
				if (oldx > 2)
					newx = oldx - 1;

				break;
			case RIGHT:
				if (oldx < WIDTH - 3)
					newx = oldx + 1;

				break;
			}
		}
		movejet(newx, newy, oldx, oldy);
		of.x = oldx = newx; // 마지막 위치를 기억한다.
		of.y = oldy = newy;
		keep_moving = 0;

	}
	else {
		jet_draw(newx, newy);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
enemy ef[100];
enemy e_head;
Bullet e_bul[E_MAX_BULLET];
Bullet e_bul_head;
void enemy_initbullet() {
	for (int i = 0; i < E_MAX_BULLET; i++) {
		e_bul[i].exist = 0;
		e_bul[i].next = NULL;
		e_bul[i].life = 0;
	}
}

void enemy_new_bullet(int x, int y, int type) {
	int i;
	for (i = 0; i < E_MAX_BULLET; i++) {
		if (e_bul[i].exist == 0) {
			e_bul[i].x = x + 1; e_bul[i].y = y;
			e_bul[i].exist = 1;
			e_bul[i].life = 1;
			e_bul[i].type = type;//1은 유도탄
			e_bul[i].next = e_bul_head.next;
			e_bul_head.next = &e_bul[i];
			break;
		}
	}
	return;
}

void enemy_bullet_move() {
	if (e_bul_head.next == NULL)
		return;
	for (Bullet* now = e_bul_head.next, *prev = &e_bul_head; now != NULL; now = now->next, prev = prev->next) {


		textcolor(0, 3);
		/*gotoxy(now->x, now->y);
		textcolor(CYAN1, CYAN1);
		printscr(" ");*/
		if (frame_count % e_bullet_frame == 0) {//프레임 조절 추가했다.
			if (now->type == 0)
				now->y += 1;
		}
		if (frame_count % e_bullet2_frame == 0) {
			if (now->type == 1) {//1은 유도탄
				now->y += 1;
				if (of.oldx - now->x < 0)
					now->x -= 1;
				else if (of.oldx - now->x > 0)
					now->x += 1;
			}
			if (now->type == 2) {//왼쪽 발사 유도탄
				now->x -= 2;
				if (of.oldy - now->y < 0)
					now->y -= 1;
				else if (of.oldy - now->y > 0)
					now->y + 1;
			}
			if (now->type == 3) {//오른쪽 발사 유도탄
				now->x += 2;
				if (of.oldy - now->y < 0)
					now->y -= 1;
				else if (of.oldy - now->y > 0)
					now->y + 1;
			}
		}




		if (now->x > WIDTH || now->x < 2 || (now->y > HEIGHT) || (now->y < 0)) {
			now->exist = 0;
			prev->next = now->next;
			now = now->next;
			return;

		}
		if (now->life != 0) {
			gotoxy(now->x, now->y);
			if (now->type == 1) {//유도탄
				textcolor(5, 3);
				printscr(BSHAPE2);
			}
			else if (now->type == 2) {//유도탄
				textcolor(5, 3);
				printscr("<");
			}
			else if (now->type == 3) {//유도탄
				textcolor(5, 3);
				printscr(">");
			}
			else if (now->type == 0) {
				textcolor(4, 3);
				printscr(BSHAPE);
			}//%s로 해서 오류생겼었음
		}
	}
}
void enemy_move() {
	static int count = 0;//유도탄 타이밍
	if (frame_count % enemy1_frame == 0) {
		for (enemy* now = e_head.next, *prev = &e_head; now != NULL; ) {
			//if (frame_count % enemy1_frame == 0) {
				/*textcolor(CYAN1, CYAN1);
				gotoxy(now->x, now->y);
				printscr("   ");*///지우기 없어도 되잖아 시벌놈아
			textcolor(0, 3);
			if (now->type == 1)
				now->y += 1;
			if (now->type == 2) {
				now->x += rand() % 3 - 1;
				now->y += 1;
				if (now->x < 2) {
					now->x += 1;
				}
				if (now->x > WIDTH - 3) {
					now->x -= 1;
				}
				//count++;//이 if문 안에 넣어놔야 프레임 조절이 된다잉
			}
			//}
			if (now->x<2 || now->x>WIDTH - 3 || now->y<0 || now->y>HEIGHT) {
				now->exist = 0;
				prev->next = now->next;
				now = now->next;
				continue;
			}



			if (now->life != 0) {
				textcolor(0, 3);
				gotoxy(now->x, now->y);
				if (now->type == 1) {
					printscr(SHAPE21);
				}
				if (now->type == 2) {
					printscr(SHAPE22);
				}
				//if (frame_count % enemy1_frame == 0) {
				if (rand() % 2 == 1) {//총 쏘는 것도 랜덤이네 ㄷㄷ
					if (count % 5 == 0) {//유도탄 쏘는 타이밍
						if (rand() % 8 < 6)
							enemy_new_bullet(now->x, now->y, 0);
						else
							enemy_new_bullet(now->x, now->y, 1);
					}
				}
				//}
				textcolor(0, 3);
			}

			now = now->next;
			prev = prev->next;
		}
		count++;
	}
	else {
		for (enemy* now = e_head.next, *prev = &e_head; now != NULL; ) {
			if (now->life != 0) {
				textcolor(0, 3);
				gotoxy(now->x, now->y);
				if (now->type == 1) {
					printscr(SHAPE21);
				}
				if (now->type == 2) {
					printscr(SHAPE22);
				}
			}
			now = now->next;
			prev = prev->next;
		}
	}

}




int ei;

void enemy_init() {
	for (int i = 0; i < 100; i++) {
		ef[i].exist = 0;
		ef[i].life = 0;
		ef[i].next = NULL;
	}
}
void enemy_new(int n) {
	int i = 0;
	int t = 0;//
	while (1) {
		i++;
		if (!ef[i].exist) {
			ef[i].exist = 1;
			ef[i].life = 3;
			ef[i].next = e_head.next;
			e_head.next = &ef[i];
			t++;
		}
		if (t == n)
			break;
	}
}
void enemy_ctrl() {
	int i;
	//static enemy* ef;
	switch (frame_count) {
	case 50: case 250: case 350: case 750: case 1050: case 1150: case 1450: case 1700: case 1710: case 1720: case 1730: case 1740: case 2350: case 2650: case 2950: case 3350: case 3800: case 3850:
	case 4300: case 5100: case 5140: case 5180: case 5400: case 5440: case 5480: case 5700: case 5800: case 5900: case 6300: case 6750: case 7000:
	case 7500: case 8400: case 9300: case 9600: case 9650: case 9700: case 9750: case 10500: case 10900: case 12000: case 12500: case 13500: case 14500: case 15500: case 16500: case 17500:
		i = 0;
		ei = 2;//ei와 type 설정만 해주면 다르게 나온다.
		enemy_new(ei);
		for (enemy* now = e_head.next; i < ei; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 2;
			i++;
		}
		break;
	case 500: case 900: case 1300: case 2200: case 2500: case 2800: case 3200: case 3600: case 4100: case 4600: case 5250: case 5450: case 6650: case 7200: case 7600:
	case 8000: case 8200: case 8300: case 8450: case 8550: case 8700: case 9000: case 9200: case 9400: case 9601: case 9801: case 10001: case 8800: case 9500: case 9900: case 11300:
	case 12300: case 13550: case 14550: case 15550: case 16550: case 17550:
		i = 0;
		ei = 3;//ei와 type 설정만 해주면 다르게 나온다.
		enemy_new(ei);
		for (enemy* now = e_head.next; i < ei; now = now->next) {
			now->x = rand() % (WIDTH - 5) + 2; now->y = 0; now->exist = 1; now->type = 1;
			i++;
		}
		break;
	}

	//for (enemy* now = e_head.next; now != NULL; now = now->next) {
	enemy_move();
	if (frame_count % enemy1_frame == 0)
		enemy_jet_check();
	//}/////////////////////////////////////////////////////////////////////////////////////////////////
}
Bullet bul[40];
Bullet head;

void jet_initBullet() {
	for (int i = 0; i < 25; i++) {
		bul[i].exist = 0;
		bul[i].next = NULL;
	}
}

void jet_newBullet(int x, int y, int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (!bul[i].exist) {
			bul[i].x = x + 1; bul[i].y = y;
			bul[i].exist = 1;
			bul[i].life = 1;
			bul[i].next = head.next;
			head.next = &bul[i];
			break;
		}
	}
	return;
}

void jet_bulletMove() {
	if (head.next == NULL)
		return;
	if (frame_count % bullet_frame == 0) {
		for (Bullet* now = head.next, *prev = &head; now != NULL; now = now->next, prev = prev->next) {

			/*gotoxy(now->x, now->y);
			textcolor(15, 0);
			//printscr(" ");*/
			now->y -= 1;
			if (now->x > WIDTH || now->x < 0 || (now->y > HEIGHT) || (now->y < 0)) {
				now->exist = 0;
				prev->next = now->next;
				now = now->next;
				return;
			}
			if (now->life == 1) {
				gotoxy(now->x, now->y);
				textcolor(15, 3);
				printscr(BSHAPE);//%s로 해서 오류생겼었음

			}
		}
	}
	else {
		for (Bullet* now = head.next, *prev = &head; now != NULL; now = now->next, prev = prev->next) {
			if (now->life == 1) {
				gotoxy(now->x, now->y);
				textcolor(15, 3);
				printscr(BSHAPE);//%s로 해서 오류생겼었음
			}
		}
	}
}

void ourBullet_enemy_check() {
	for (Bullet* now = head.next; now != NULL; now = now->next) {
		if (now->life == 0) {
			continue;
		}
		for (enemy* e_now = e_head.next; e_now != NULL; e_now = e_now->next) {
			if (e_now->life == 0) {
				continue;
			}
			if ((now->y == e_now->y) && (now->x - e_now->x <= 2) && (now->x - e_now->x >= 0)) {
				now->life = 0;
				score++;
				explode(now->x, now->y);
				e_now->life--;
				if (e_now->life <= 0)
					e_now->life = 0;

			}
		}
	}
}
void eb_o_check() {
	for (Bullet* now = e_bul_head.next; now != NULL; now = now->next) {
		if (now->life == 0) {
			continue;
		}

		if ((now->y == of.y) && (now->x - of.x <= 2) && (now->x - of.x >= 0)) {
			now->life = 0;
			explode(now->x, now->y);
			of.life--;
			printscr("\a");
			if (of.life <= 0)
				of.life = 0;
		}
	}
}

void enemy_jet_check() {
	for (enemy* now = e_head.next; now != NULL; now = now->next) {
		if (now->life == 0) {
			continue;
		}

		if ((now->y == of.y) && (now->x - of.x <= 2) && (now->x - of.x >= -2)) {
			explode(now->x, now->y);
			of.life--;
			printscr("\a");
			score++;
			now->life = 0;
			if (of.life <= 0)
				of.life = 0;
			/*if (now->life <= 0)
				now->life = 0;*/

		}
	}
}

void i_o_check() {
	for (item* now = i_head.next; now != NULL; now = now->next) {
		if (now->life == 0) {
			continue;
		}

		if ((now->y == of.y) && (now->x - of.x <= 2) && (now->x - of.x >= -2)) {
			now->life = 0;
			//explode(now->x, now->y);
			if (now->type == 1) {
				of.life++;
			}
			if (now->type == 2) {
				max_bullet++;
			}
			if (now->type == 3) {
				max_bullet += 2;
				of.life += 3;
			}
			if (max_bullet > 25)
				max_bullet--;
		}
	}
}
void ob_bos_check() {
	for (Bullet* now = head.next; now != NULL; now = now->next) {
		if (now->life == 0) {
			continue;
		}

		if ((now->y == bos.y + 3) && (now->x - bos.x <= 14) && (now->x - bos.x >= 0)) {
			now->life = 0;
			score++;
			explode(now->x, now->y);
			bos.life--;
			if (bos.life <= 0)
				bos.life = 0;


		}
	}
}
void o_bos_check() {
	if ((of.y == bos.y + 3) && (of.x - bos.x <= 14) && (of.x - bos.x >= 0)) {
		explode(of.x, of.y);
		printscr("\a");
		if (frame_count % 10 == 0) {

			of.life--;

		}
	}
}
void explode(int x, int y) {


	gotoxy(x, y);

	textcolor(0, 4);
	printscr(" ");
	textcolor(0, 1);
	printscr(" ");
	textcolor(0, 0);
	//gotoxy(x, y);
	//printscr("  ");

}
void show_info() {
	char s1[10];
	char s2[10];
	char s3[10];
	char s4[10];
	char s5[10];
	textcolor(4, 15);
	gotoxy(2, HEIGHT + 1);
	sprintf(s1, "생명: %d", of.life);
	printscr(s1);

	textcolor(1, 15);
	gotoxy(13, HEIGHT + 1);
	sprintf(s2, "총알: %02d", max_bullet);
	printscr(s2);

	textcolor(0, 15);
	printscr("(max:25)");
	textcolor(0, 15);
	gotoxy(33, HEIGHT + 1);
	sprintf(s3, "점수: %d", score);
	printscr(s3);
	gotoxy(44, HEIGHT + 1);
	//sprintf(s4, "시간: %d초", frame_count / 100);
	//printscr(s4);
}

int main() {
START:
	SetConsoleTitle(TEXT("1945 Game"));
	system("mode con cols=82 lines=47");

	//ending();
	scr_init();
	keep_moving = 0;
	frame_count = 0;
	score = 50;//적을 죽이면 오른다 
	srand(time(NULL));
	//기초 설정
	of.life = LIFE;
	i_head.next = NULL;
	e_head.next = NULL;
	head.next = NULL;
	e_bul_head.next = NULL;
	int chase = 6;


	removeCursor();

	int title[6][16] =
	{ {0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1},
	 {0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0},
	 {0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,1},
	 {0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1},
	 {0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1},
	 {0,1,1,1,0,1,1,1,0,0,1,0,0,1,1,1},
	};
	int ti = 0;
	cls(15, 15);
	int c1, c2;
	int ch;
	while (1) {//타이틀 만들기
		scr_clear();

		c1 = ti++ % 15 + 1;
		textcolor(c1, 0);
		for (int y = 0; y < 6; y++) {
			for (int x = 0; x < 16; x++) {
				gotoxy(22 + x * 2, 12 + y);
				if (title[y][x] == 1)
					printscr("■");
			}
		}
		gotoxy(31, 20);
		textcolor(15, 0);
		printscr("press (s) to start");
		textcolor(0, 15);
		gotoxy(10, 22); printscr("▣KEY INSTRUCTION");
		gotoxy(13, 24); printscr("△");
		gotoxy(11, 25); printscr("◁"); printscr("▽"); printscr("▷"); printscr(" : MOVE");
		gotoxy(16, 27); printscr("Z : FIRE");

		gotoxy(50, 22); printscr("▣ITEM INSTRUCTION");
		gotoxy(53, 24); printscr("[P] : MAX BULLET+");
		gotoxy(53, 25); printscr("[L] : LIFE+");
		gotoxy(53, 26); printscr("+S+ : MAX BULLET+,LIFE+3");



		if (kbhit()) {
			if (getch() == 's') {
				break;
			}
		}
		Sleep(500);
		scr_switch();
	}//타이틀 끝

	jet_initBullet();
	init_boss();
	enemy_init();
	init_item();


	int bos_i = 0;
	for (int i = 0; i < 2; i++) {
		scr_switch();
		drawbox(15, 3);
		scr_clear();
	}


	scr_clear();
	while (1) {

		scr_clear();
		frame_count++;
		drawbox2(0, CYAN1);

		jet_ctrl();


		of.oldx = of.x;
		of.oldy = of.y;//이게 없으니까 유도탄이 동작을 안 함

		i_o_check();
		/*if (score < 100 && frame_count < 18000) {
			enemy_jet_check();
		}
		else {
			o_bos_check();
		}*///이거 지워도 되는거 아님?
		//eb_o_check();
		enemy_bullet_move();
		eb_o_check();
		jet_bulletMove();
		i_o_check();
		item_ctrl();//두 ctrl은 안에서 프레임을 설정한다. 
		//if (score < 100 && frame_count < 18000)
			//ourBullet_enemy_check();


		if (score < BOSS_SCORE && frame_count < 18000) {
			enemy_ctrl();
			ourBullet_enemy_check();
			enemy_jet_check();
		}
		//else if (score >= 100 || frame_count >= 18000)
			//ob_bos_check();
		//item_ctrl();//두 ctrl은 안에서 프레임을 설정한다. 
		//i_o_check();

		if (score >= BOSS_SCORE || frame_count >= 18000) {
			if (bos_i == 0) {
				boss_come_on_stage();
				chase = 3;//유도탄 추적 관리/////////////////////////////////////////////
				bos_i = 1;
			}
			//if (frame_count % enemy1_frame == 0)
			boss_ctrl();
			o_bos_check();
			ob_bos_check();
			if (bos.life == 0) {
				Sleep(1500);
				scr_release();
				ending();
			}
			//ob_bos_check();
		}
		show_info();
		if (of.life <= 0 || frame_count >= 30000) {
			lose();
			goto START;
		}

		//Sleep(7.5);
		scr_switch();
	}
}


