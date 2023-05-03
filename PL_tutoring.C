#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
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

#define WIDTH 62   //맵 가로 크기
#define HEIGHT 48  // 맵 세로 크기
#define UX 30  //시작 유저의 x좌표
#define UY 45  //시작 유저의 y좌표

#define SPECIAL1 0XE0
#define SPECIAL2 0x00
#define SPACE 0x20
#define UP 0x48   //방향키 위
#define DOWN 0x50  //방향기 다운
#define LEFT 0x4b  //방향기 왼쪽
#define RIGHT 0x4d  //방향키 오른쪽
int p1_frame_sync = 4; //플레이어의 이동속도
int keep_moving = 1;  //1:계속이동
int called = 0;       
int oldx = UX, oldy = UY; // 플레이어의 old 좌표
int newx = UX, newy = UY; //플레이어의 new 좌표
int time_out = 100; //
int frame_count = 0; // game 진행 frame count 로 속도 조절용으로 사용된다.
int Delay = 10;      //while문 속도조절용으로 사용된다

//커서 지우는 함수
void removeCursor(void) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//커서위치이동 함수
void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };  //COORD = x값과 y값을 멤버로 가지는 구조체타입
	  // pos = COORD 구조체 변수 이름 ,(x,y좌표를 담고있음)
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//텍스트 색 배경색 변경함수
void textcolor(int fg_color, int bg_color)
{
	//fg_color = 글자색 , bg_color = 배경 색
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);

}

//화면 지우는 고 색변경 함수
void cls(int text_color, int bg_color) {
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color); //배열 cmd에 문자열 COLOR 숫자숫자 넣어줌
	system(cmd);
}

//게임종료까지 남은시간 보여주는 함수
void show_time(int remain_time)
{
	textcolor(YELLOW2, BLACK);
	gotoxy(20, 52);
	printf("게임종료까지 남은시간 : %3d", remain_time);

}

//플레이어 그리기
void playerdraw(int x, int y) {
	textcolor(GREEN1, BLACK);
	gotoxy(x, y);
	printf("<=★=>");
}

//플레이어 지우기
void playererase(int x, int y) {
	gotoxy(x, y);
	printf("      ");
}
void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
	int y;
	int x;

	// 세로를 그리는 for문
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
	//가로를 그리는 for문
	for (x = x1; x <= x2; x += 2) {
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
}

void playermove(unsigned char ch) {

	int move_flag = 0;
	static unsigned char last_ch = 0;  //static이므로 전역변수라고 생각하면 된다 (메모리에 계속 올라가있음)


	if (called == 0) {  //플레이어 호출 함수가 처음 불렸다면 ?
		removeCursor();
		playerdraw(oldx, oldy);  //플레이어를 글려줌
		called = 1;
	}

	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:  //방향키 위쪽이 눌렸다면?
		if (oldy > 25)  //플레이어의 y좌표가 25보다 크다면?
			newy = oldy - 1;    //플레이어의 새로윈 y좌표는 현재위치  Y위치 -1
		else {   //벽을 만났다면 반대로 움직이는 로직 
			//else문 움직이면 반대로 안움직이고 벽만나면 그냥 막힌다
			newy = oldy + 1;
			last_ch = DOWN; //벽을 만났기 때문에 DOWN해줘서 내려가게 해준다
		}
		move_flag = 1; //계속 움직이게 해줌
		break;
	case DOWN:
		if (oldy < HEIGHT - 3)  //플레이어의 Y좌표가 맵의 크기 -3 보다 작다면
			newy = oldy + 1;   //플레이어의 새로운 Y좌표는 현재 Y위치 +1
		else {  //벽만나는 로직
			newy = oldy - 1;
			last_ch = UP;
		}
		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 2)  //플레이어의 X좌표가 2보다 크다면 왼쪽으로 이동 가능
			newx = oldx - 1;
		else {  //그게아니라면 반대로 이동
			newx = oldx + 1;
			last_ch = RIGHT;
		}
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 6)  //플레이어의 X좌표가 맵 크기-6보다 작다면 오른쪽으로 이동 가능
			newx = oldx + 1;
		else { //반대로 이동
			newx = oldx - 1;
			last_ch = LEFT;
		}
		move_flag = 1;
		break;
	}
	if (move_flag) {
		playererase(oldx, oldy); //플레이어를 예전위치를 지우고
		playerdraw(newx, newy); // 새로운 위치에서 플레이어 그려준다
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy; // 마지막 위치를 기억한다.
	
	}
}
void gamestart() {
	int run_time, start_time, remain_time, last_remain_time;
	unsigned char ch;


	start_time = time(NULL);  //게임 시작전 시간
	last_remain_time = remain_time = time_out;
	while (1) {
		run_time = time(NULL) - start_time;  // 현재 시간 - 게임시작 전 시간 
		remain_time = time_out - run_time;  //remain_time = 게임종료까지 남은시간
		if (remain_time < last_remain_time) {
			show_time(remain_time); // 게임종료까지 남으시간을 출력하는 함수 호출
			last_remain_time = remain_time;
		}

		if (remain_time == 0) // 남은시간이 0이면 while문 종료로 게임종료
			break;

		if (kbhit() == 1) {  //키보드가 눌렸다면 
			ch = getch();
			if (ch == SPECIAL1 || ch == SPECIAL2) {  //그 키보드 값이 위에 Define 으로 선언한 값이라면
				ch = getch();
				switch (ch) {
				case UP: case DOWN: case LEFT: case RIGHT:   //그 키보드 값이 방향키라면
					playermove(ch);    //플레이어를 움직이기 위한 player move 함수 호출
				
					break;
				default :
					if (frame_count % p1_frame_sync == 0)
						playermove(0);
				}
			}
		}
		//방향키를 한번만 눌러도 계속 움직이게 하기 위해서 키가 안눌러져있어도 playermove함수 호출
		else {
			//속도조절 로직
			if (frame_count % p1_frame_sync == 0)  //frame_count 초기화 값은 0이다. 아래 while문을 한번 돌때마다 frame_count의 값은 1씩 증가한다
				   //p1_frame_sync가 4라고 가정하자 -> while문이 4번돌아 frame_count가 4가 되어야  playermove함수를 호출한다. 
				playermove(0);
		}
		Sleep(Delay); // Delay 값으로 while문 속도조절 가능
		frame_count++;  //frame_count 값으로 여러함수들의 호출 속도를 조절하여 플레이어 이동속도 조절가능
	}
	
}
void help() {
	textcolor(15, 0);
	system("cls");  //화면 지우기
	gotoxy(5, 10);
	printf("방향키를 통해서 지렁이를 움직일 수 있습니다\n");
	gotoxy(7, 11);
	system("pause");

}
void startmenu() {
	int a;
	while (1) {
		system("cls"); //화면 지우기

		draw_box(0, 0, 62, 48, "*");
		textcolor(2, 0);
		gotoxy(30, 10);
		printf("지렁이 게임");
		gotoxy(30, 13);
		printf("게임시작 : (1)");
		gotoxy(31, 16);
		printf("도움말 : (2)");
		gotoxy(32, 19);
		printf("종료 : (3)");

		
		gotoxy(30, 25);
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


int main() {
	startmenu();
}
