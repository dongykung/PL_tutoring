#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// 색상 정의
#define BLACK   0
#define BLUE1   1
#define GREEN1   2
#define CYAN1   3
#define RED1   4
#define MAGENTA1 5
#define YELLOW1   6
#define GRAY1   7
#define GRAY2   8
#define BLUE2   9
#define GREEN2   10
#define CYAN2   11
#define RED2   12
#define MAGENTA2 13
#define YELLOW2   14
#define WHITE   15

#define PLAYER1 'O' // player1 표시
#define PLAYER2 'X' // player2 표시
#define BLANK ' ' // ' ' 로하면 흔적이 지워진다 

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define ENTER 0x0D

#define UP2      'w'//player2 는 AWSD 로 방향키 대신
#define DOWN2   's'
#define LEFT2   'a'
#define RIGHT2   'd'
#define SPECIAL1 0XE0
#define SPECIAL2 0x00
#define SPACE 0x20

#define WIDTH 120
#define HEIGHT 40

#define STAR1 'O' // player1 표시
#define STAR2 'X' // player2 표시

int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.
int p1_frame_sync = 4; //플레이어의 이동속도
int p1_frame_sync_start = 0;

int Delay = 100; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int time_out = 120; // 제한시간
int score[2] = { 0 };
int golds[WIDTH][HEIGHT] = { 0 }; // 1이면 Gold 있다는 뜻
int goldinterval = 3; // GOLD 표시 간격
int called[2];



void playerdraw();
void player1(unsigned char ch);
void show_time(int remain_time);

void removeCursor(void) { // 커서를 안보이게 한다

    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void puttri(int x, int y, char ch)
{
    gotoxy(x, y);
    putchar(ch);
}
void erasetri(int x, int y)
{
    gotoxy(x, y);
    putchar(BLANK);
}

void textcolor(int fg_color, int bg_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
    char cmd[100];
    system("cls");
    sprintf(cmd, "COLOR %x%x", bg_color, text_color);
    system(cmd);
}

void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
    int x, y;
    for (x = x1; x <= x2; x += 2) {
        gotoxy(x, y1);
        printf("%s", ch);
        gotoxy(x, y2);
        printf("%s", ch);
    }
    for (y = y1; y <= y2; y += 1) {
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

void draw_map1() {   // 상점 맵

    draw_box(0, 1, 118, 39, "＄");
}
void start() {
    cls(BLACK, WHITE);
    unsigned char ch;
    int run_time, start_time, remain_time, last_remain_time;
    int gold_time;

START:
    /*init_game();*/
    start_time = time(NULL);
    last_remain_time = remain_time = time_out;
    draw_box(0, 1, 118, 39, "▩");
    while (1) {
        // 캐릭터 그리기 일단은 *
        // 캐릭터가 상하좌우로 움직이게. (어떤 모양의 캐릭터로 할지 고민)
        // 
        //gotoxy(15,18);
        //playerdraw();
        run_time = time(NULL) - start_time;
        remain_time = time_out - run_time;
        if (remain_time < last_remain_time) {
            show_time(remain_time); // 시간이 변할때만 출력
            last_remain_time = remain_time;
        }
        if (remain_time == 0) // 시간 종료
            break;
        if (_kbhit() == 1) {  // 키보드가 눌려져 있으면
            char c1;
            c1 = _getch(); // key 값을 읽는다
            if (c1 == '\0') // VS 의 NULL 값은 Error이다.
                continue;
            else
                ch = c1;
            // ESC 누르면 프로그램 종료
            if (ch == ESC)
                exit(0);
            if (ch == SPECIAL1) { // 만약 특수키
                // 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
                ch = _getch();
                // Player1은 방향키로 움직인다.
                switch (ch) {
                case UP:
                case DOWN:
                case LEFT:
                case RIGHT:
                    player1(ch);//player1만 방향 전환
                    break;
                default: // 방향 전환이 아니면
                    if (frame_count % p1_frame_sync == 0)
                        player1(0);
                }
            }
        }
        else {
            // keyboard 가 눌려지지 않으면 계속 움직인다.
            // 이동중이던 방향으로 계속 이동
            if (frame_count % p1_frame_sync == 0)
                player1(0);
        }

        Sleep(Delay); // Delay 값을 줄이고
    }
}
void help() {
    cls(BLACK, WHITE);
    draw_box(32, 2, 88, 34, "▩");
    unsigned char ch;
    while (1) {
        gotoxy(55, 5);
        printf("헌팅 RPG게임");
        gotoxy(38, 12);
        printf("① 몬스터를 잡아서 돈을 획득하세요.");
        gotoxy(38, 14);
        printf("② 상점에서 코인을 사용해서");
        gotoxy(38, 15);
        printf("     스킬과 능력을 업그레이드 하세요.");
        gotoxy(38, 17);
        printf("③ 마지막으로 보스를 처리해서 게임을 승리하세요!!");
        gotoxy(52, 30);
        printf("< < 메뉴 : ENTER > >");
        if (_kbhit()) {
            ch = _getch();

            if (ch == ENTER) {
                break;
            }
        }
    }
}
void playerdraw(int x, int y) {
    textcolor(RED1, BLACK);
    gotoxy(x, y);
    printf("*");
    textcolor(BLACK, WHITE);
}
void playererase(int x, int y) {
    gotoxy(x, y);
    textcolor(RED1, BLACK);
    printf(" ");
    textcolor(BLACK, WHITE);
}
void player1(unsigned char ch)
{
    static int oldx = 60, oldy = 10, newx = 60, newy = 10;
    int move_flag = 0;
    static unsigned char last_ch = 0;

    if (called[0] == 0) { // 처음 또는 Restart
        oldx = 60, oldy = 10, newx = 60, newy = 10;
        //putstar(oldx, oldy, STAR1);
        playerdraw(oldx, oldy);
        called[0] = 1;
        last_ch = 0;
        ch = 0;
    }
    /*if (keep_moving && ch == 0)
       ch = last_ch;
    last_ch = ch;*/

    switch (ch) {
    case UP:
        if (oldy > 1) // 0 은 Status Line
            newy = oldy - 1;
        else { // 벽에 부딛치면 방향을 반대로 이동
            newy = oldy + 1;
            last_ch = DOWN;
            newy = HEIGHT - 2;
        }
        move_flag = 1;
        break;
    case DOWN:
        if (oldy < HEIGHT - 2)
            newy = oldy + 1;
        else {
            newy = oldy - 1;
            last_ch = UP;
            newy = 1;
        }
        move_flag = 1;
        break;
    case LEFT:
        if (oldx > 0)
            newx = oldx - 1;
        else {
            newx = oldx + 1;
            last_ch = RIGHT;
        }
        move_flag = 1;
        break;
    case RIGHT:
        if (oldx < WIDTH - 1)
            newx = oldx + 1;
        else {
            newx = oldx - 1;
            last_ch = LEFT;
        }
        move_flag = 1;
        break;
    }
    if (move_flag) {
        playererase(oldx, oldy); // 마지막 위치의 * 를 지우고
        playerdraw(newx, newy); // 새로운 위치에서 * 를 표시한다.
        oldx = newx; // 마지막 위치를 기억한다.
        oldy = newy;
    }
}
void menuImg() {

    textcolor(WHITE, RED1);
    gotoxy(25, 4);
    printf("┏━━┓   ┏━━┓┏━━┓   ┏━━┓┏━━━━━━━━━┓┏━━━━━━━━━━┓┏━━━━┓┏━━━━━━━━━┓┏━━━━━━━━━┓ \n");
    gotoxy(25, 5);
    printf("┃  ┃   ┃  ┃┃  ┃   ┃  ┃┃  ┏━━━┓  ┃┗━━━┓  ┏━━━┛┗┓  ┏┛┃  ┏━━━┓  ┃┃  ┏━━━━━━┛ \n");
    gotoxy(25, 6);
    printf("┃  ┗━━━┛  ┃┃  ┃   ┃  ┃┃  ┃   ┃  ┃    ┃  ┃     ┃  ┃ ┃  ┃   ┃  ┃┃  ┃ ┏━━━━┓ \n");
    gotoxy(25, 7);
    printf("┃  ┏━━━┓  ┃┃  ┃   ┃  ┃┃  ┃   ┃  ┃    ┃  ┃     ┃  ┃ ┃  ┃   ┃  ┃┃  ┃ ┗━┓  ┃ \n");
    gotoxy(25, 8);
    printf("┃  ┃   ┃  ┃┃  ┗━━━┛  ┃┃  ┃   ┃  ┃    ┃  ┃    ┏┛  ┗┓┃  ┃   ┃  ┃┃  ┗━━━┛  ┃ \n");
    gotoxy(25, 9);
    printf("┗━━┛   ┗━━┛┗━━━━━━━━━┛┗━━┛   ┗━━┛    ┗━━┛    ┗━━━━┛┗━━┛   ┗━━┛┗━━━━━━━━━┛ \n");
    textcolor(BLACK, WHITE);

    /*printf("┏━┓ ┏━┓┏━┓ ┏━┓┏━━━━━┓┏━━━━━┓┏━┓┏━━━━━┓┏━━━━━┓ \n");
    gotoxy(37, 5);
    printf("┃ ┗━┛ ┃┃ ┃ ┃ ┃┃ ┏━┓ ┃┗━┓ ┏━┛┃ ┃┃ ┏━┓ ┃┃ ┏━━━┓ \n");
    gotoxy(37, 6);
    printf("┃ ┏━┓ ┃┃ ┗━┛ ┃┃ ┃ ┃ ┃  ┃ ┃  ┃ ┃┃ ┃ ┃ ┃┃ ┗━┛ ┃ \n");
    gotoxy(37, 7);
    printf("┗━┛ ┗━┛┗━━━━━┛┗━┛ ┗━┛  ┗━┛  ┗━┛┗━┛ ┗━┛┗━━━━━┛ \n");*/
}

void showCharacter() {
    removeCursor();
    printf("└ (＠ヮ＠)つ¤=[]▣───\n");   //캐릭터


    //┌┐┘└─│     ┏ ┓ ┗ ┛ ━ ┃
       /*┏━━┓   ┏━━┓            ┏━┓  ┏━┓┏━━━━━━┓┏━━━━━━━┓┏━━━━━┓ ┏━━━━━━┓┏━━━━━━┓
       ┃  ┗━━━┛  ┃            ┃ ┃  ┃ ┃┃ ┏━━┓ ┃┗━━┓ ┏━━┛┗━┓ ┏━┛ ┃ ┏━━┓ ┃┃  ┏━━━┓
       ┃  ┏━━━┓  ┃            ┃ ┗━━┛ ┃┃ ┃  ┃ ┃   ┃ ┃   ┏━┛ ┗━┓ ┃ ┃  ┃ ┃┃  ┗━┛ ┃
       ┗━━┛   ┗━━┛            ┗━━━━━━┛┗━┛  ┗━┛   ┗━┛   ┗━━━━━┛ ┗━┛  ┗━┛┗━━━━━━┛*/
}
void menuSelect2(int x, int y) { // 메뉴선택 표시
    /*▶◀*/
    gotoxy(x - 7, y);
    printf("\u25B6");
    gotoxy(x + 5, y);
    printf("\u25C0");
}
void eraseMenuSelect2(int x, int y) { // 메뉴선택 문자 삭제
    gotoxy(x - 7, y);
    printf("  ");
    gotoxy(x + 5, y);
    printf("  ");
}
void menuSelect() {   // 첫 화면
    system("cls");
    int a;
    int x = 61, y = 20;
    unsigned char ch;

    while (1) {
        //system("cls"); //화면 지우기
        int c1, c2;
        menuImg();
        textcolor(WHITE, BLACK);

        gotoxy(57, 20);
        printf("게임시작");
        gotoxy(58, 23);
        printf("도움말");
        gotoxy(59, 26);
        printf("종료");

        do { // 색을 변경하면서 Game Over 출력
            c1 = rand() % 16;
        } while (c1 == 0);
        textcolor(c1, BLACK); // 배경은 검은색으로, 텍스트만 색 변경
        if (_kbhit()) {
            ch = _getch();
            if (ch == UP) {
                eraseMenuSelect2(x, y);
                y -= 3;
                if (y < 20) {
                    y = 26;
                }
            }
            if (ch == DOWN) {
                eraseMenuSelect2(x, y);
                y += 3;
                if (y > 26) {
                    y = 20;
                }
            }
            textcolor(WHITE, BLACK);
            if (ch == ENTER) {
                if (y == 20) {
                    start();
                }
                else if (y == 23) {
                    help();
                    system("cls");
                }
                else {
                    exit(0);
                }

            }
        }
        menuSelect2(x, y);
        Sleep(Delay);
    }
}
void show_time(int remain_time)
{
    textcolor(YELLOW2, GRAY2);
    gotoxy(57, 0);
    printf("남은시간 : %03d", remain_time);
    textcolor(BLACK, WHITE);
}

int main() {
    srand(time(NULL));
    textcolor(WHITE, BLACK);
    removeCursor();
    menuSelect();   // 초기 메뉴 실행

    return 0;
}

