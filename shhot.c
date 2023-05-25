#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>
#define ITEM "<H>"
#define SPEEDITEM "<S>"
#define POWERITEM "<P>"
#define MISILEITEM "<M>"
#define UX 30  //시작 유저의 x좌표
#define UY 45  //시작 유저의 y좌표
#define WIDTH 62
#define HEIGHT 48
#define MAXBULLET 6//플레이어의 최대 총알 수
#define MAXENEMY 6 //최대 적 수
#define MAXENEMYBULLET 20 // 적 최대 총알 수
#define MAXBOSSBULLET 4
#define MAXBARRIER 10
#define MAXMISILE 1
//색
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
//색
#define SPECIAL1 0XE0
#define SPECIAL2 0x00
#define SPACE 0x20
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define TRUE 1
#define FALSE 0

struct {
	int exist;
	int x, y;
}Bullet[MAXBULLET];
struct {
	int exist;
	int x, y;
}Misile[MAXMISILE];
struct {
	int exist;
	int x, y;
}BossBullet[MAXBOSSBULLET];
struct {
	int exist;
	int x, y;
	int number;
	int move;
	int type;
}Enemy[MAXENEMY];

struct {
	int exist;
	int x, y;
	int frame;
	int stay;
}EnemyBullet[MAXENEMYBULLET];
struct {
	int exist;
	int x, y;
}Barrier[MAXBARRIER];
char name[10];
char name2[10];
int heart = 3;//플레이어의 목숨
int misile = 0;
int bossheart = 150;
int called = 0;
int score = 0;//게임점수
int bossbulletuse = 1;
int bossspeedup = 15;
int bossbarrieruse = 2;//2초마다 보스스테이지 장애물 출력
int items[WIDTH][HEIGHT] = { 0 };
int items_count = 0;
int items_use = 35; //아이템 나타나는 시간(목숨 아이템)
int misileitems[WIDTH][HEIGHT] = { 0 };
int misileitems_count = 0;
int misileitems_use = 20;
int speeditems[WIDTH][HEIGHT] = { 0 };
int speeditems_count = 0;
int speeditems_use = 23; //아이템 나타나는 시간(스피드 아이템)
int p = 0; //플레이어 총알강화 변수
int power[WIDTH][HEIGHT] = { 0 };
int power_count = 0;
int power_use = 25; //아이템 나타나는 시간(파워 아이템)
int boss_frame_sync = 15;  //보스 프레임단위
int bossbullet_frame_sync = 8; //보스총알속도
int item_frame_sync = 25;//아이템 움직이는 속도
int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.
int p1_frame_sync = 4; //플레이어의 이동속도
int p1_frame_sync_count = 0;
int time_out = 100; //보스가 나타나기까지의 시간
int enemyuse = 1;  //1초마다 적 생성
int enemybulletuse = 1; //적 총알 1초마다 생성
int keep_moving = 1;  //1:계속이동
int bullet_frame_sync = 4;
int misile_frame_sync = 2;
int enemy_frame_sync = 6;
int enemybullet_frame_sync = 15;
int barrier_frame_sync = 10;
int speedupem = 10;
int Delay = 10;
int oldx = UX, oldy = UY; // 플레이어의 old 좌표
int newx = UX, newy = UY; //플레이어의 new 좌표
int bossx = 30, bossy = 10;
char* Enemyunit[] = { "⊙★⊙","＠▒＠","♨▣♨","◐▼◑" };

//맵,색
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
void removeCursor(void) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
	int y;
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
}
void draw_box2(int x1, int x2, int y, char* ch)
{
	int x;
	for (x = x1; x <= x2; x += 2) {
		gotoxy(x, y);
		printf("%s", ch);
	}
}
void help() {
	system("cls");  //화면 지우기
	gotoxy(5, 10);
	printf("<1> 방향키 이동 및 총알발사:SPACE.");
	gotoxy(5, 12);
	printf("<2> 게임 시작 시 체력3이 지급되며 <H>아이템을 먹을 시");
	gotoxy(9, 13);
	printf("체력이 1 회복되며 최대체력 수:4");
	gotoxy(5, 15);
	printf("<3> <S>아이템을 먹을시 플레이어와 총알 속도 증가");
	gotoxy(9, 16);
	printf("죽어도 능력이 사라지지 않습니다.");
	gotoxy(5, 18);
	printf("<4> <P>아이템 먹을시 총알이 2배로나가며");
	gotoxy(9, 19);
	printf("보스에게 2의피해를 입힘(죽을시 능력 사라짐)");
	gotoxy(5, 21);
	printf("<5> <M>아이템 먹을시 미사일 1개 획득<보스에게 15피해>");
	gotoxy(5, 23);
	printf("<6> 적의 총알은 플레이어를 따라오며 10초마다 속도증가");
	gotoxy(5, 25);
	printf("<7> 보스시간초가 0이 될 시 보스등장!!");
	gotoxy(9, 27);
	printf("보스스테이지에서는 플레이어를 따라다니는 장애물 생성");
	gotoxy(5, 29);
	printf("<8> 보스의 체력이0 이 될 시 게임 클리어");
	gotoxy(12, 30);
	system("pause");

}
//플레이어 및 플레이어 총알,미사일
void playerdraw(int x, int y) {
	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("<=★=>");
}
void playererase(int x, int y) {
	gotoxy(x, y);
	printf("      ");
}
void playermove(unsigned char ch) {

	int move_flag = 0;
	static unsigned char last_ch = 0;


	if (called == 0) {
		removeCursor();
		playerdraw(oldx, oldy);
		called = 1;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 25)
			newy = oldy - 1;
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 3)
			newy = oldy + 1;
		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 2)
			newx = oldx - 1;
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 6)
			newx = oldx + 1;
		move_flag = 1;
		break;
	}
	if (move_flag) {
		playererase(oldx, oldy);
		playerdraw(newx, newy); // 새로운 위치에서 플레이어 표시
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;
		if (items[newx][newy]) {
			heart++;
			items[newx][newy] = 0;
			items_count--;
		}
		if (speeditems[newx][newy]) {
			speeditems[newx][newy] = 0;
			speeditems_count--;
			if (p1_frame_sync > 2)
				p1_frame_sync--;
			if (bullet_frame_sync > 2)
				bullet_frame_sync--;
		}
		if (power[newx][newy]) {
			p++;
			power[newx][newy] = 0;
			power_count--;
		}
		if (misileitems[newx][newy]) {
			misile++;
			misileitems[newx][newy] = 0;
			misileitems_count--;
		}
	}
}
void DrawBullet(int i) {
	textcolor(GREEN1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("│");
}
void DrawBullet2(int i) {
	textcolor(GREEN1, BLACK);
	gotoxy(Bullet[i].x, Bullet[i].y); printf("││");
}
void EraseBullet(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("  ");
}
void EraseBullet2(int i) {
	gotoxy(Bullet[i].x, Bullet[i].y); printf("    ");
}
void bulletmove() {
	int i;
	if (p == 0) {
		for (i = 0; i < MAXBULLET; i++) {
			if (Bullet[i].exist == TRUE) {
				EraseBullet(i);
				if (Bullet[i].y == 0) {
					Bullet[i].exist = FALSE;
				}
				else {
					Bullet[i].y--;
					DrawBullet(i);
				}
			}
		}
	}
	else {
		for (i = 0; i < MAXBULLET; i++) {
			if (Bullet[i].exist == TRUE) {
				EraseBullet2(i);
				if (Bullet[i].y == 0) {
					Bullet[i].exist = FALSE;
				}
				else {
					Bullet[i].y--;
					DrawBullet2(i);
				}
			}
		}
	}
}
void drawmisile(int i) {
	gotoxy(Misile[i].x, Misile[i].y - 2); printf("∀∀");
	gotoxy(Misile[i].x, Misile[i].y - 1); printf("ΠΠ");
	gotoxy(Misile[i].x, Misile[i].y); printf("ΠΠ");
}
void erasemisile(int i) {
	gotoxy(Misile[i].x, Misile[i].y - 2); printf("    ");
	gotoxy(Misile[i].x, Misile[i].y - 1); printf("    ");
	gotoxy(Misile[i].x, Misile[i].y); printf("    ");
}
void misilemove() {
	int i;
	for (i = 0; i < MAXMISILE; i++) {
		if (Misile[i].exist == TRUE) {
			erasemisile(i);
			if (Misile[i].y - 2 == 0) {
				Misile[i].exist = FALSE;
			}
			else {
				Misile[i].y--;
				drawmisile(i);
			}
		}
	}
}
//적 및 적 총알 움직임
void EnemyBulletshow() {
	int j;
	int random = rand() % MAXENEMY;
	for (j = 0; j < MAXENEMYBULLET && EnemyBullet[j].exist == TRUE; j++) {}
	if (j != MAXENEMYBULLET && Enemy[random].exist == TRUE) {
		EnemyBullet[j].x = Enemy[random].x + 2;
		EnemyBullet[j].y = Enemy[random].y + 1;
		EnemyBullet[j].exist = TRUE;
	}
}
void EnemyBulletdraw(int i) {
	textcolor(RED2, BLACK);
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf("*");
}
void EnemyBulleterase(int i) {
	gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
	printf(" ");
}
void EnemyBulletMove() {
	int random;
	random = 1;
	for (int i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == TRUE) {
			EnemyBulleterase(i);
			if (EnemyBullet[i].y > HEIGHT - 3) {
				EnemyBullet[i].exist = FALSE;
			}
			else {
				if (random) {
					if (EnemyBullet[i].x <= newx)
						EnemyBullet[i].x++;
					else
						EnemyBullet[i].x--;
				}
				EnemyBullet[i].y++;
				EnemyBulletdraw(i);
			}
		}
	}
}
void showenemy() {
	int i, location, direct, hieght;
	location = rand() % 2;
	direct = 5 + rand() % 55;
	hieght = 1 + rand() % 8;
	for (i = 0; i < MAXENEMY && Enemy[i].exist == TRUE; i++) {}
	if (i != MAXENEMY) {
		if (location == 1) {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = 1;
		}
		else {
			Enemy[i].x = direct;
			Enemy[i].y = hieght;
			Enemy[i].move = -1;
		}
		Enemy[i].type = rand() % 4;
		Enemy[i].exist = TRUE;
	}
}
void Enemymove() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == TRUE) {
			if (Enemy[i].type == -1) {
				gotoxy(Enemy[i].x - 1, Enemy[i].y);
				printf("          ");
				Enemy[i].exist = FALSE;
				continue;
			}
			if (Enemy[i].x > 52) {
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("    ");
				Enemy[i].x -= 1;
				Enemy[i].move = -1;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf(" ");

			}
			else if (Enemy[i].x <= 3) {
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("    ");
				Enemy[i].move = 1;
				Enemy[i].x += 1;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf(" ");
			}
			else {
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf("     ");
				Enemy[i].x += Enemy[i].move;
				gotoxy(Enemy[i].x, Enemy[i].y);
				printf(Enemyunit[Enemy[i].type]);
				printf("    ");
			}
		}
	}
}
//충돌구현
void Enemyfall() {
	int i;
	for (i = 0; i < MAXENEMY; i++) {
		if (Enemy[i].exist == FALSE || Enemy[i].type == -1)
			continue;
		for (int j = 0; j < MAXBULLET; j++) {
			if (Enemy[i].y == Bullet[j].y && abs(Enemy[i].x - Bullet[j].x) <= 7) {
				gotoxy(Bullet[j].x, Bullet[j].y);
				printf("   ");
				Bullet[j].exist = FALSE;
				Enemy[i].type = -1;
				score += 10;
				break;
			}

		}

	}
}
void playerfall() {
	int i;
	for (i = 0; i < MAXENEMYBULLET; i++) {
		if (EnemyBullet[i].exist == FALSE)
			continue;
		if (EnemyBullet[i].y == newy && abs(EnemyBullet[i].x - newx) <= 3) {
			EnemyBullet[i].exist = FALSE;
			gotoxy(EnemyBullet[i].x, EnemyBullet[i].y);
			printf("    ");
			heart--;
			p = 0;
			system("cls");
		}
	}
}
void bossfall() {
	int i;
	for (i = 0; i < MAXBULLET; i++) {
		if (Bullet[i].exist == FALSE)
			continue;
		if (bossy + 6 == Bullet[i].y && abs(bossx - Bullet[i].x) <= 12) {
			gotoxy(Bullet[i].x, Bullet[i].y);
			printf("   ");
			Bullet[i].exist = FALSE;
			if (p == 0) {
				bossheart--;
			}
			else {
				bossheart -= 2;
			}
			break;
		}
	}
	for (i = 0; i < MAXMISILE; i++) {
		if (Misile[i].exist == FALSE)
			continue;
		if (bossy + 6 == Misile[i].y && abs(bossx - Misile[i].x) <= 12) {
			gotoxy(Misile[i].x, Misile[i].y); printf("    ");
			gotoxy(Misile[i].x, Misile[i].y - 1); printf("    ");
			gotoxy(Misile[i].x, Misile[i].y - 2); printf("    ");
			Misile[i].exist = FALSE;
			bossheart -= 15;
		}
	}
}
void playerbossfall() {
	int i;
	for (i = 0; i < MAXBOSSBULLET; i++) {
		if (BossBullet[i].exist == FALSE)
			continue;
		if (BossBullet[i].y == newy && abs(BossBullet[i].x - newx) <= 3) {
			BossBullet[i].exist = FALSE;
			gotoxy(BossBullet[i].x, BossBullet[i].y);
			printf("    ");
			heart--;
			p = 0;
			system("cls");

		}
	}
}
void playerbarrierfall() {
	int i;
	for (i = 0; i < MAXBARRIER; i++) {
		if (Barrier[i].exist == FALSE)
			continue;
		if (Barrier[i].y == newy && abs(Barrier[i].x - newx) <= 3) {
			Barrier[i].exist = FALSE;
			gotoxy(Barrier[i].x, Barrier[i].y);
			printf("    ");
			heart--;
			p = 0;
			system("cls");
		}
	}
}
//아이템 및 정보출력
void info() {
	textcolor(YELLOW2, BLACK);
	gotoxy(25, 49); printf("플레이어:%s", name);
	textcolor(WHITE, BLACK);
	gotoxy(45, 50); printf("점수: %d", score);
	gotoxy(28, 50); printf("미사일 개수:%2d", misile);
	gotoxy(25, 51); printf("보스체력:%3d", bossheart);
	gotoxy(15, 50);
	printf("목숨:");
	if (heart >= 4) {
		heart = 4;
		gotoxy(20, 50); printf("       ");
		gotoxy(20, 50); printf("♥♥♥♥");
	}
	else if (heart == 3) {
		gotoxy(20, 50); printf("       ");
		gotoxy(20, 50); printf("♥♥♥");
	}
	else if (heart == 2) {
		gotoxy(20, 50); printf("       ");
		gotoxy(20, 50); printf("♥♥");
	}
	else if (heart == 1) {
		gotoxy(20, 50); printf("       ");
		gotoxy(20, 50); printf("♥");
	}
	else {
		gotoxy(20, 50);
		printf("  ");
	}

}
void showitem() {
	int x, y;
	x = 7 + rand() % 40;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(ITEM);
	items[x][y] = 1;
	items_count++;
	textcolor(YELLOW2, BLACK);
}
void moveitem() {
	int x, y, dx, dy, nx, ny;
	int newitems[WIDTH][HEIGHT] = { 0 };


	// gold 수가 없을 수 있다.
	if (items_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (items[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					items_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   "); // erase gold
					gotoxy(nx, ny);
					printf(ITEM);
					newitems[nx][ny] = 1; // 이동된 golds의 좌표
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(items, newitems, sizeof(newitems)); // 한번에 gold 위치를 조정한다.
}
void showspeeditem() {
	int x, y;
	x = 7 + rand() % 40;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(SPEEDITEM);
	speeditems[x][y] = 1;
	speeditems_count++;
	textcolor(YELLOW2, BLACK);
}
void movespeeditem() {
	int x, y, dx, dy, nx, ny;
	int newspeeditems[WIDTH][HEIGHT] = { 0 };


	// gold 수가 없을 수 있다.
	if (speeditems_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (speeditems[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					speeditems_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   "); // erase gold
					gotoxy(nx, ny);
					printf(SPEEDITEM);
					newspeeditems[nx][ny] = 1; // 이동된 golds의 좌표
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(speeditems, newspeeditems, sizeof(newspeeditems)); // 한번에 speed 위치를 조정한다.
}
void powershow() {
	int x, y;
	x = 7 + rand() % 40;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(POWERITEM);
	power[x][y] = 1;
	power_count++;
	textcolor(YELLOW2, BLACK);
}
void powermove() {
	int x, y, dx, dy, nx, ny;
	int newpoweritems[WIDTH][HEIGHT] = { 0 };

	if (power_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (power[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					power_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   ");
					gotoxy(nx, ny);
					printf(POWERITEM);
					newpoweritems[nx][ny] = 1;
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(power, newpoweritems, sizeof(newpoweritems)); // 한번에 위치를 조정한다.
}
void misileitem() {
	int x, y;
	x = 7 + rand() % 40;
	y = 2;
	textcolor(YELLOW2, BLACK);
	gotoxy(x, y);
	printf(MISILEITEM);
	misileitems[x][y] = 1;
	misileitems_count++;
	textcolor(YELLOW2, BLACK);
}
void misileitemmove() {
	int x, y, dx, dy, nx, ny;
	int newmisileitem[WIDTH][HEIGHT] = { 0 };
	// gold 수가 없을 수 있다.
	if (misileitems_count == 0)
		return;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (misileitems[x][y]) {
				dx = rand() % 3 - 1; // -1 0 1
				dy = 1;
				nx = x + dx;
				ny = y + dy;
				if (nx == WIDTH - 5) nx = WIDTH - 6;
				if (nx < 7) nx = 8;
				if (ny < 1) ny = 1;
				if (ny > HEIGHT - 1) {
					gotoxy(x, y);
					printf("   ");
					misileitems_count--;
				}
				else {
					gotoxy(x, y);
					textcolor(YELLOW2, BLACK);
					printf("   ");
					gotoxy(nx, ny);
					printf(MISILEITEM);
					newmisileitem[nx][ny] = 1; // 이동된 misile item 좌표
					textcolor(YELLOW2, BLACK);
				}
			}
		}
	}
	memcpy(misileitems, newmisileitem, sizeof(newmisileitem)); // 한번에 위치를 조정한다.
}
void show_time(int remain_time)
{
	textcolor(YELLOW2, BLACK);
	gotoxy(20, 52);
	printf("보스까지남은시간 : %3d", remain_time);

}
//보스 함수들
void bossdraw(int x, int y) {
	gotoxy(x, y); printf("V    V");
	gotoxy(x + 1, y + 1); printf("V  V");
	gotoxy(x + 2, y + 2); printf("VV");
	gotoxy(x - 10, y + 3); printf("V▼▼▼V □□□□ V▼▼▼V");
	gotoxy(x - 9, y + 4); printf("V▼▼V  ▼※※▼  V▼▼V");
	gotoxy(x - 7, y + 5); printf(" V     ▼※▼     V");
	gotoxy(x + 2, y + 6); printf("▼");
}
void bosserase(int x, int y) {
	gotoxy(x - 2, y - 1); printf("         ");
	gotoxy(x - 2, y); printf("                     ");
	gotoxy(x - 2, y + 1); printf("                 ");
	gotoxy(x - 1, y + 2); printf("                   ");
	gotoxy(x - 13, y + 3); printf("                                 ");
	gotoxy(x - 12, y + 4); printf("                                 ");
	gotoxy(x - 10, y + 5); printf("                            ");
	gotoxy(x, y + 6); printf("           ");
	gotoxy(x, y + 7); printf("    ");

}
void BossBulleterase(int b) {
	gotoxy(BossBullet[b].x, BossBullet[b].y);
	printf("        ");
}
void BossBulletdraw(int b) {
	gotoxy(BossBullet[b].x, BossBullet[b].y);
	printf("*");
}
void bossbulletmove() {
	int b;
	int random = rand() % MAXBOSSBULLET;
	int rm = rand() % 3 - 1;
	for (b = 0; b < MAXBOSSBULLET; b++) {
		if (BossBullet[b].exist == TRUE) {
			BossBulleterase(b);
			if (BossBullet[b].y > HEIGHT - 3) {
				BossBullet[b].exist = FALSE;
			}
			else {
				BossBullet[b].y++;
				BossBulletdraw(b);
			}
		}
	}
}
void bossbulletshow() {
	int k;
	for (k = 0; k < MAXBOSSBULLET && BossBullet[k].exist == TRUE; k++) {}
	if (k != MAXBOSSBULLET) {
		BossBullet[k].x = bossx;
		BossBullet[k].y = bossy + 7;
		BossBullet[k].exist = TRUE;
	}
}
//장애물 함수
void drawbarrier(int i) {
	gotoxy(Barrier[i].x, Barrier[i].y);
	printf("⊙");
}
void erasebarrier(int i) {
	gotoxy(Barrier[i].x, Barrier[i].y);
	printf("  ");
}
void barriershow() {
	int i;
	int random;
	random = 5 + rand() % 45;
	for (i = 0; i < MAXBARRIER && Barrier[i].exist == TRUE; i++) {}
	if (i != MAXBARRIER) {
		Barrier[i].x = random;
		Barrier[i].y = 2;
		Barrier[i].exist = TRUE;
	}
}
void barriermove() {
	int random;
	random = 1;
	for (int i = 0; i < MAXBARRIER; i++) {
		if (Barrier[i].exist == TRUE) {
			erasebarrier(i);
			if (Barrier[i].y > HEIGHT - 3) {
				Barrier[i].exist = FALSE;
			}
			else {
				if (random) {
					if (Barrier[i].x < newx)
						Barrier[i].x++;
					else
						Barrier[i].x--;
				}
				Barrier[i].y++;
				drawbarrier(i);
			}
		}
	}
}
//게임 초기화 함수
void init_game() {
	int x, y;
	srand(time(NULL));
	called = 0;
	for (x = 0; x < WIDTH - 2; x++) {
		for (y = 0; y < HEIGHT; y++) {
			items[x][y] = 0;
			speeditems[x][y] = 0;
			power[x][y] = 0;
			misileitems[x][y] = 0;
		}
	}
	for (x = 0; x < MAXENEMYBULLET; x++)
		EnemyBullet[x].exist = FALSE;
	for (x = 0; x < MAXENEMY; x++)
		Enemy[x].exist = FALSE;
	for (x = 0; x < MAXBULLET; x++)
		Bullet[x].exist = FALSE;
	for (x = 0; x < MAXBARRIER; x++)
		Barrier[x].exist = FALSE;
	for (x = 0; x < MAXBOSSBULLET; x++)
		BossBullet[x].exist = FALSE;
	for (x = 0; x < MAXMISILE; x++)
		Misile[x].exist = FALSE;
	newx = 30;
	newy = 45;
	keep_moving = 1, misile = 0;
	time_out = 100;
	heart = 3;
	speedupem = 10;
	bossheart = 150, called = 0, score = 0, p = 0;
	p1_frame_sync = 4, bullet_frame_sync = 4;
	enemybullet_frame_sync = 15;
	enemybulletuse = 1;
	boss_frame_sync = 15;
	bossbullet_frame_sync = 8;
	system("cls");
	removeCursor();




}
//게임스타트함수
void gamestart() {
	int run_time, start_time, remain_time, last_remain_time;
	int enemy_time, enemybullettime, items_time, speeditems_time, speedupemtime, powertime, misiletime;
	int bossrun_time, boss_start_time, boss_bulletshow, boss_barrier;
	unsigned char ch;
	int move_flag;
	int i;
START:
	init_game();
	move_flag = 0;
	boss_bulletshow = 0;
	boss_barrier = 0;
	enemy_time = 0; //
	enemybullettime = 0; //
	items_time = 0;
	speeditems_time = 0;
	speedupemtime = 0;
	powertime = 0;
	misiletime = 0;
	start_time = time(NULL);
	last_remain_time = remain_time = time_out;
	show_time(remain_time);
	//일반 스테이지
	while (1) {
		info();
		Enemyfall();
		playerfall();


		if (heart == 0) {
			break;
		}

		run_time = time(NULL) - start_time;
		remain_time = time_out - run_time;
		if (remain_time < last_remain_time) {
			show_time(remain_time); // 시간이 변할때만 출력
			last_remain_time = remain_time;
		}
		if (remain_time == 0) // 시간 종료  보스등장
			break;

		if (run_time > items_time && (run_time % items_use == 0)) {
			showitem();//x초마다 목숨아이템생성
			items_time = run_time; // 마지막 item 표시 시간 기억
		}
		if (run_time > misiletime && (run_time % misileitems_use == 0)) {
			misileitem();//x초마다 미사일아이템생성
			misiletime = run_time; // 마지막 item 표시 시간 기억
		}

		if (run_time > enemy_time && (run_time % enemyuse == 0)) {
			showenemy();;//1초마다 적 생산
			enemy_time = run_time; // 마지막 item 표시 시간 기억
		}
		if (run_time > speeditems_time && (run_time % speeditems_use == 0)) {
			showspeeditem();//x초마다 스피드아이템생성
			speeditems_time = run_time; // 마지막 speeditem 표시 시간 기억
		}

		if (run_time > enemybullettime && (run_time % enemybulletuse == 0)) {
			EnemyBulletshow();//1초마다 적 총알 출력
			enemybullettime = run_time;
		}
		if (run_time > powertime && (run_time % power_use == 0)) {
			powershow();// 강화아이템출력
			powertime = run_time;
		}
		if (run_time > speedupemtime && (run_time % speedupem == 0)) {
			if (enemybullet_frame_sync > 5)//10초마다 적의 총알 속도가 빨라짐
				enemybullet_frame_sync--;
			speedupemtime = run_time;
		}
		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:
					playermove(ch);
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
			if (ch == SPACE) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 1;
					Bullet[i].y = newy - 1;
					Bullet[i].exist = TRUE;
				}
			}
			if (ch == 'd') {
				if (misile != 0) {
					for (i = 0; i < MAXMISILE && Misile[i].exist == TRUE; i++) {}
					if (i != MAXMISILE) {
						Misile[i].x = newx + 1;
						Misile[i].y = newy - 1;
						Misile[i].exist = TRUE;
						misile--;
					}
				}
			}
		}
		else {
			if (frame_count % p1_frame_sync == 0)
				playermove(0);
		}
		if (frame_count % bullet_frame_sync == 0)
			bulletmove();
		if (frame_count % misile_frame_sync == 0) {
			textcolor(YELLOW2, BLACK);
			misilemove();
		}
		if (frame_count % enemy_frame_sync == 0) {
			textcolor(RED2, BLACK);
			Enemymove();
		}
		if (frame_count % enemybullet_frame_sync == 0)
			EnemyBulletMove();
		if (frame_count % item_frame_sync == 0)
			moveitem();
		if (frame_count % item_frame_sync == 0)
			movespeeditem();
		if (frame_count % item_frame_sync == 0)
			powermove();
		if (frame_count % item_frame_sync == 0)
			misileitemmove();
		Sleep(Delay); // Delay 값을 줄이고
		frame_count++;// frame_count 값으로 속도 조절을 한다.
	}
	if (heart == 0) {
		while (1) {
			int c1, c2;
			do {
				c1 = rand() % 16;
				c2 = rand() % 16;
			} while (c1 == c2);
			textcolor(c1, c2);
			gotoxy(25, 20);
			printf("**게임 오버**");
			gotoxy(17, 21);
			textcolor(WHITE, BLACK);
			printf("Hit (R) to Restart (Q) to Quit");
			Sleep(300);
			if (kbhit()) {
				ch = getch();
				if (ch == 'r' || ch == 'q')
					break;
			}
		}
		if (ch == 'r')
			goto START;
		if (ch == 'q')
			exit(0);
	}
	system("cls");
	gotoxy(28, 28);
	printf("보스등장");
	gotoxy(28, 29);
	printf("보스등장");
	gotoxy(28, 30);
	printf("보스등장");
	Sleep(3000);
	system("cls");

	boss_start_time = time(NULL);
	//보스 스테이지
	while (1) {

		bossrun_time = time(NULL) - boss_start_time;
		info();
		bossfall();
		playerbossfall();
		playerbarrierfall();
		if (heart == 0) {
			break;
		}
		if (bossheart <= 0)
			break;
		if (bossrun_time > boss_bulletshow && (bossrun_time % bossbulletuse == 0)) {
			bossbulletshow();//  1초마다 보스총알 생성
			boss_bulletshow = bossrun_time; // 마지막 총알 표시 기억
		}
		if (bossrun_time > boss_barrier && (bossrun_time % bossbarrieruse == 0)) {
			barriershow();//  1초마다 보스총알 생성
			boss_barrier = bossrun_time; // 마지막 총알 표시 기억
		}
		if (bossrun_time > powertime && (bossrun_time % power_use == 0)) {
			powershow();// 강화아이템출력
			powertime = bossrun_time;
		}
		if (bossrun_time > misiletime && (bossrun_time % misileitems_use == 0)) {
			misileitem();//x초마다 미사일아이템생성
			misiletime = bossrun_time; // 마지막 item 표시 시간 기억
		}
		if (bossrun_time > items_time && (bossrun_time % items_use == 0)) {
			showitem();//x초마다 목숨아이템생성
			items_time = bossrun_time; // 마지막 item 표시 시간 기억
		}
		if (bossrun_time > bossspeedup && (bossrun_time % bossspeedup == 0)) {
			if (boss_frame_sync > 10)//15초마다 보스의 속도가 빨라짐
				boss_frame_sync--;
			bossspeedup = bossrun_time;
		}
		if (bossrun_time > bossspeedup && (bossrun_time % bossspeedup == 0)) {
			if (bossbullet_frame_sync > 3)//15초마다 보스의 총알 속도가 빨라짐
				bossbullet_frame_sync--;
			bossspeedup = bossrun_time;
		}
		if (frame_count % boss_frame_sync == 0) {
			int randomx = rand() % 3 - 1; //-1,0,1
			int randomy = rand() % 3 - 1;//-1.0,1
			textcolor(RED2, BLACK);
			if (bossx > 40)
				bossx -= 1;
			if (bossx < 12) {
				gotoxy(bossx - 11, bossy + 3); printf("               ");
				gotoxy(bossx - 11, bossy + 2); printf("               ");
				gotoxy(bossx - 11, bossy + 4); printf("               ");
				bossx += 1;
			}
			if (bossy > 15) {
				gotoxy(bossx - 10, 21);
				printf("                             ");
				bossy -= 1;
			}
			if (bossy < 7) {
				gotoxy(bossx - 12, bossy + 3);
				printf("                          ");
				bossy += 1;
			}
			bosserase(bossx, bossy);
			bossdraw(bossx + randomx, bossy + randomy);
			bossx += randomx, bossy += randomy;
		}

		if (kbhit() == 1) {
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:
					playermove(ch);
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
					break;
				default:
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
			if (ch == SPACE) {
				for (i = 0; i < MAXBULLET && Bullet[i].exist == TRUE; i++) {}
				if (i != MAXBULLET) {
					Bullet[i].x = newx + 1;
					Bullet[i].y = newy - 1;
					Bullet[i].exist = TRUE;
				}
			}
			if (ch == 'd') {
				if (misile != 0) {
					for (i = 0; i < MAXMISILE && Misile[i].exist == TRUE; i++) {}
					if (i != MAXMISILE) {
						Misile[i].x = newx + 1;
						Misile[i].y = newy - 1;
						Misile[i].exist = TRUE;
						misile--;
					}
				}
			}
		}
		else {
			if (frame_count % p1_frame_sync == 0)
				playermove(0);
		}
		if (frame_count % bullet_frame_sync == 0)
			bulletmove();  //플레이어의 총알 속도제어
		if (frame_count % misile_frame_sync == 0) {
			textcolor(YELLOW2, BLACK);
			misilemove();
		}
		if (frame_count % bossbullet_frame_sync == 0) {
			textcolor(RED2, BLACK);
			bossbulletmove();//보스의 총알 속도제어
		}
		if (frame_count % barrier_frame_sync == 0) {
			textcolor(RED2, BLACK);
			barriermove();//장애물의 속도제어
		}
		if (frame_count % item_frame_sync == 0)
			powermove();
		if (frame_count % item_frame_sync == 0)
			moveitem();
		if (frame_count % item_frame_sync == 0)
			misileitemmove();
		Sleep(Delay); // Delay 값을 줄이고
		frame_count++;// frame_count 값으로 속도 조절을 한다.
	}
	while (1) {
		int c1, c2;
		do {
			c1 = rand() % 16;
			c2 = rand() % 16;
		} while (c1 == c2);
		textcolor(c1, c2);
		gotoxy(25, 20);
		if (heart == 0) {
			printf("**게임 오버**");
		}
		if (bossheart <= 0) {
			printf("게임 클리어");
		}
		gotoxy(17, 21);
		textcolor(WHITE, BLACK);
		printf("Hit (R) to Restart (Q) to Quit");
		Sleep(300);
		if (kbhit()) {
			ch = getch();
			if (ch == 'r' || ch == 'q')
				break;
		}
	}
	if (ch == 'r')
		goto START;
	if (ch == 'q')
		exit(0);
}
void startmenu() {
	int a;
	while (1) {
		system("cls"); //화면 지우기

		gotoxy(13, 5);
		printf("■■■■    ■■■■■     ■■■■");
		gotoxy(13, 6);
		printf("■              ■         ■");
		gotoxy(13, 7);
		printf("■■■■        ■         ■  ■■■");
		gotoxy(13, 8);
		printf("      ■        ■         ■    ■");
		gotoxy(13, 9);
		printf("■■■■        ■         ■■■■");
		gotoxy(25, 15);
		printf("게임시작(1)");
		gotoxy(26, 20);
		printf("도움말(2)");
		gotoxy(27, 25);
		printf("종료(3)");
		gotoxy(20, 28);
		gotoxy(20, 30);
		printf("번호를 입력하시오:");
		scanf_s("%d", &a);
		if (a == 1) {
			gamestart();
			break;
		}
		else if (a == 2) {
			help();   //도움말 함수
		}
		else if (a == 3) {
			printf("종료");
			break;
		}
	}

}



void main() {
	srand(time(NULL));
	textcolor(WHITE, BLACK);
	removeCursor();
	gotoxy(8, 20);
	printf("이름을 입력하세요<입력 후 영문변환>:");
	gets(name);
	startmenu();

}
