#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>

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
#define NUM_STARS 6
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
typedef struct {
    int x;
    int y;
    int exist;
} Star;

void initializeStars(Star stars[], int numStars) {
    srand(time(NULL));
    stars[0].x = 5; stars[0].y = 3;
    stars[1].x = 5; stars[1].y = 20;
    stars[2].x = 35; stars[2].y = 24;
    stars[3].x = 20; stars[3].y = 29;
    stars[4].x = 55; stars[4].y = 20;
    stars[5].x = 40; stars[5].y = 30;
 
}

void printStars(Star stars[], int numStars) {
    for (int i = 0; i < numStars; i++) {
        if (stars[i].exist == 1){
            int r = rand() % 16;
            gotoxy(stars[i].x, stars[i].y);
            textcolor(r, 0);
            printf("*");
        }
        else {
            gotoxy(stars[i].x, stars[i].y);
            printf(" ");
        }
    }

    fflush(stdout);
}

void updateStars(Star stars[], int numStars) {
    for (int i = 0; i < numStars; i++) {
        if (rand() % 2 == 0) {
            stars[i].exist = !stars[i].exist;
        }
    }
}

void startpage() {

  

    Star stars[NUM_STARS];
    initializeStars(stars, NUM_STARS);

    unsigned char ch;
    //draw_box2(2, 2, 60, 50);//작은 틀
    int color1, color2;
    while (1) {
        //system("cls"); //화면 지우기   
        color1 = rand() % 15 + 1;
        color2 = rand() % 16;

     
        textcolor(YELLOW1, BLACK);
        gotoxy(25, 32);
        printf("게임시작(1)");
        gotoxy(26, 34);
        printf("도움말(2)");
        gotoxy(27, 36);
        printf("종료(3)");
        gotoxy(20, 38);
        printf("번호를 입력하시오:");
        ////scanf_s("%d", &a);

        /*
        gotoxy(5, 3);
        textcolor(WHITE, BLACK);
        printf("1");
        gotoxy(5, 20);
        printf("2");
        gotoxy(35, 24);
        printf("3");
        gotoxy(20, 29);
        printf("4");
        gotoxy(55, 20);
        printf("5");
        gotoxy(40, 30);
        printf("6");
        */
        updateStars(stars, NUM_STARS);
        printStars(stars, NUM_STARS);
        textcolor(color1, BLACK);
        gotoxy(5, 4);
        printf("              ■                  ■              \n");
        gotoxy(5, 5);
        printf("                ■              ■                \n");
        gotoxy(5, 6);
        printf("                ■■          ■■                \n");
        gotoxy(5, 7);
        printf("              ■■■■■■■■■■■              \n");
        gotoxy(5, 8);
        printf("              ■■■■■■■■■■■              \n");
        gotoxy(5, 9);
        printf("          ■■■    ■■■■■    ■■■          \n");
        gotoxy(5, 10);
        printf("          ■■■    ■■■■■    ■■■          \n");
        gotoxy(5, 11);
        printf("      ■■■■■■■■■■■■■■■■■■■      \n");
        gotoxy(5, 12);
        printf("      ■■    ■■■■■■■■■■■    ■■      \n");
        gotoxy(5, 13);
        printf("      ■■    ■■■■■■■■■■■    ■■      \n");
        gotoxy(5, 14);
        printf("      ■■    ■                  ■    ■■      \n");
        gotoxy(5, 15);
        printf("              ■                  ■              \n");
        gotoxy(5, 16);
        printf("                ■■■      ■■■                \n");

        Sleep(500);
        /*gotoxy(5, 20);
        textcolor(WHITE, BLACK);
        printf(" ");*/
        textcolor(color1, BLACK);
        gotoxy(5, 4);
        printf("              ■                  ■              \n");
        gotoxy(5, 5);
        printf("                ■              ■                \n");
        gotoxy(5, 6);
        printf("                ■■          ■■                \n");
        gotoxy(5, 7);
        printf("    ■■      ■■■■■■■■■■■      ■■    \n");
        gotoxy(5, 8);
        printf("    ■■      ■■■■■■■■■■■      ■■    \n");
        gotoxy(5, 9);
        printf("    ■■  ■■■    ■■■■■    ■■■  ■■    \n");
        gotoxy(5, 10);
        printf("    ■■■■■■    ■■■■■    ■■■■■      \n");
        gotoxy(5, 11);
        printf("      ■■■■■■■■■■■■■■■■■■        \n");
        gotoxy(5, 12);
        printf("              ■■■■■■■■■■■              \n");
        gotoxy(5, 13);
        printf("              ■■■■■■■■■■■              \n");
        gotoxy(5, 14);
        printf("              ■                  ■              \n");
        gotoxy(5, 15);
        printf("              ■                  ■              \n");
        gotoxy(5, 16);
        printf("        ■■■                      ■■■        \n");
        Sleep(500);

    }
}
int main() {
   
    
    /*
    while (1) {
        system("cls");

        updateStars(stars, NUM_STARS);
        printStars(stars, NUM_STARS);

        Sleep(500);
    }*/
    startpage();
    return 0;
}
